
#ifndef AF_PLUGIN_INTEFACE_H
#define AF_PLUGIN_INTEFACE_H

#include <afSystem.h>
#include <string>
#include <afPath.h>
#include <afEnums.h>

#ifndef _WIN32
  #include <unistd.h>
#endif
#include <dlfcn.h>

#include <sys/stat.h>
#include <iostream>

using namespace std;

// Forward declaration outside ambf namespace
class GLFWwindow;

namespace ambf {


// Forward declaration
class afWorld;
class afModel;
class afBaseObject;

typedef afWorld* afWorldPtr;
typedef afModel* afModelPtr;
typedef afBaseObject* afBaseObjectPtr;

class afWorldAttributes;
class afModelAttributes;
class afBaseObjectAttributes;

typedef afWorldAttributes* afWorldAttribsPtr;
typedef afModelAttributes* afModelAttribsPtr;
typedef afBaseObjectAttributes* afBaseObjectAttribsPtr;

template<class T>
class afPluginBase
{
public: typedef T* TPtr;

public: afPluginBase()
    {
        this->dlHandle = nullptr;
    }

public: virtual ~afPluginBase()
    {
    }

public: string getFilename() const
    {
        return this->filename;
    }

public: string getHandle() const
    {
        return this->handleName;
    }

public: static TPtr Create(const std::string &_filename,
                           const std::string &_name)
    {
        TPtr result = nullptr;
        // PluginPtr result;
        struct stat st;
        bool found = false;
        string fullname, filename(_filename);
        list<string>::iterator iter;
        list<string> pluginPaths;
        pluginPaths.push_back("/home/adnan/ambf/build-debug/ambf_plugins");

#ifdef __APPLE__
        // This is a hack to work around issue #800,
        // error loading plugin libraries with different extensions
        {
            size_t soSuffix = filename.rfind(".so");
            if (soSuffix != std::string::npos)
            {
                const std::string macSuffix(".dylib");
                filename.replace(soSuffix, macSuffix.length(), macSuffix);
            }
        }
#elif _WIN32
        // Corresponding windows hack
        {
            // replace .so with .dll
            size_t soSuffix = filename.rfind(".so");
            if (soSuffix != std::string::npos)
            {
                const std::string winSuffix(".dll");
                filename.replace(soSuffix, winSuffix.length(), winSuffix);
            }
            size_t libPrefix = filename.find("lib");
            if (libPrefix == 0)
            {
                // remove the lib prefix
                filename.erase(0, 3);
            }
        }
#endif  // ifdef __APPLE__

        for (iter = pluginPaths.begin();
             iter!= pluginPaths.end(); ++iter)
        {
            fullname = (*iter)+string("/")+filename;
            fullname = afPath(fullname).c_str();
            if (stat(fullname.c_str(), &st) == 0)
            {
                found = true;
                break;
            }
        }

        if (!found)
            fullname = filename;

        fptr_union_t registerFunc;
        std::string registerName = "CreatePlugin";

        void *dlHandle = dlopen(fullname.c_str(), RTLD_LAZY|RTLD_GLOBAL);
        if (!dlHandle)
        {
            cerr << "Failed to load plugin " << fullname << ": "
                  << dlerror() << "\n";
            return result;
        }

        registerFunc.ptr = dlsym(dlHandle, registerName.c_str());

        if (!registerFunc.ptr)
        {
            cerr << "Failed to resolve " << registerName
                  << ": " << dlerror();
            return result;
        }

        // Register the new controller.
        result = registerFunc.func();
        result->dlHandle = dlHandle;

        result->handleName = _name;
        result->filename = filename;

        return result;
    }

public: afPluginType GetType() const
    {
        return this->type;
    }

protected: afPluginType type;

protected: string filename;

protected: string handleName;

private: typedef union
    {
        T *(*func)();
        void *ptr;
    } fptr_union_t;

private: void *dlHandle;
};


///
/// \brief The afWorldPlugin class
///
class afSimulatorPlugin: public afPluginBase<afSimulatorPlugin>{
public:
    afSimulatorPlugin(){
        this->type = afPluginType::SIMULATOR;
    }

    virtual void init(int argc, char** argv, const afWorldPtr a_afWorld){}
    virtual void keyboardUpdate(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods){}
    virtual void mouseBtnsUpdate(GLFWwindow* a_window, int a_button, int a_action, int a_modes){}
    virtual void mousePosUpdate(GLFWwindow* a_window, double x_pos, double y_pos){}
    virtual void mouseScrollUpdate(GLFWwindow* a_window, double x_pos, double y_pos){}
    virtual void graphicsUpdate(){}
    virtual void physicsUpdate(double dt){}
    virtual void reset(){}
    virtual bool close(){}
};


///
/// \brief The afWorldPlugin class
///
class afWorldPlugin: public afPluginBase<afWorldPlugin>{
public:
    afWorldPlugin(){
        this->type = afPluginType::WORLD;
    }

    virtual void init(const afWorldPtr a_afWorld, const afWorldAttribsPtr a_worldAttribs){}
    virtual void onModelAdd(const afModelPtr a_modelPtr){}
    virtual void onModelRemoval(const afModelPtr a_modelPtr){}
    virtual void onObjectAdd(const afBaseObjectPtr a_objectPtr){}
    virtual void onObjectRemoval(const afBaseObjectPtr a_objectPtr){}
    virtual void graphicsUpdate(){}
    virtual void physicsUpdate(double dt){}
    virtual void reset(){}
    virtual bool close(){}
};


///
/// \brief The afWorldPlugin class
///
class afModelPlugin: public afPluginBase<afModelPlugin>{
public:
    afModelPlugin(){
        this->type = afPluginType::MODEL;
    }

    virtual void init(const afModelPtr a_afModel, const afModelAttribsPtr a_modelAttribs){}
    virtual void onObjectAdd(const afBaseObjectPtr a_objectPtr){}
    virtual void onObjectRemoval(const afBaseObjectPtr a_objectPtr){}
    virtual void graphicsUpdate(){}
    virtual void physicsUpdate(double dt){}
    virtual void reset(){}
    virtual bool close(){}
};

///
/// \brief The afWorldPlugin class
///
class afObjectPlugin: public afPluginBase<afObjectPlugin>{
public:
    afObjectPlugin(){
        this->type = afPluginType::OBJECT;
    }

    virtual void init(const afBaseObjectPtr a_afObjectPtr, afBaseObjectAttribsPtr a_objectAttribs){}
    virtual void graphicsUpdate(){}
    virtual void physicsUpdate(double dt){}
    virtual void reset(){}
    virtual bool close(){}
};

}

#define AF_REGISTER_SIMULATOR_PLUGIN(PluginClass) \
    extern "C" ambf::afSimulatorPlugin* CreatePlugin() \
    {\
        return new PluginClass(); \
    }

#define AF_REGISTER_MODEL_PLUGIN(PluginClass) \
    extern "C" ambf::afModelPlugin* CreatePlugin() \
    {\
        return new PluginClass(); \
    }

#define AF_REGISTER_WORLD_PLUGIN(PluginClass) \
    extern "C" ambf::afWorldPlugin* CreatePlugin() \
    {\
        return new PluginClass(); \
    }

#define AF_REGISTER_OBJECT_PLUGIN(PluginClass) \
    extern "C" ambf::afObjectPlugin* CreatePlugin() \
    {\
        return new PluginClass(); \
    }

#endif // AF_PLUGIN_INTEFACE_H