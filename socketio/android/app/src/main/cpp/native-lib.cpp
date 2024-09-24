#include <jni.h>
#include <string>
#include <condition_variable>
#include <chrono>
#include <thread>
#include <pthread.h>
#include <android/log.h>

#include "sio_client.h"

#define  LOG_TAG    "SocketIOCPP"
#define  LOGUNK(...)  __android_log_print(ANDROID_LOG_UNKNOWN,LOG_TAG,__VA_ARGS__)
#define  LOGDEF(...)  __android_log_print(ANDROID_LOG_DEFAULT,LOG_TAG,__VA_ARGS__)
#define  LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGF(...)  __android_log_print(ANDROID_FATAL_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGS(...)  __android_log_print(ANDROID_SILENT_ERROR,LOG_TAG,__VA_ARGS__)

#define SERVER_HOSTNAME "localhost"
#define SERVER_PORT 5002
#define NAMESPACE "/controller"

//
// references:
//  - https://groups.google.com/g/android-ndk/c/UP5v_S0BDdY
//  - https://developer.android.com/reference/android/os/Build
//
const char *const GetDeviceName(JNIEnv *env) {
    // Fetch the device model as name for now
    jclass build_class = env->FindClass("android/os/Build");
    jfieldID model_id = env->GetStaticFieldID(build_class, "DEVICE", "Ljava/lang/String;");
    jstring model_obj  = (jstring)env->GetStaticObjectField(build_class, model_id);
    const char *deviceName = env->GetStringUTFChars(model_obj, 0);
    return deviceName;
}

const char *const GetModelName(JNIEnv *env) {
    // Fetch the device model as name for now
    jclass build_class = env->FindClass("android/os/Build");
    jfieldID model_id = env->GetStaticFieldID(build_class, "MODEL", "Ljava/lang/String;");
    jstring model_obj  = (jstring)env->GetStaticObjectField(build_class, model_id);
    const char *deviceName = env->GetStringUTFChars(model_obj, 0);
    return deviceName;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_crazinglab_socketiocpp_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

    std::string hello = "[Server]\n - ";
    hello += "[";
    hello += GetDeviceName(env);
    hello += "] ";
    hello += SERVER_HOSTNAME;
    hello += ":" + std::to_string(SERVER_PORT);
    return env->NewStringUTF(hello.c_str());
}

static pthread_t app_thread;
static void *server_thread(void *userdata);
static std::string device_name;
static std::string model_name;

extern "C" JNIEXPORT void JNICALL
Java_com_crazinglab_socketiocpp_MainActivity_initThread(
        JNIEnv* env,
        jobject /* this */) {

    LOGI("initThread(...)\n");
    device_name = GetDeviceName(env);
    model_name  = GetModelName(env);
    pthread_create(&app_thread, nullptr, server_thread, nullptr);
}

std::mutex _lock;
std::condition_variable_any _cond;
bool connect_finish = false;
class connection_listener
{
    sio::client &handler;

public:
    connection_listener(sio::client &handler) : handler(handler) {
    };

    ~connection_listener(void) {
    };

    void on_connected()
    {
        LOGI("%s", __PRETTY_FUNCTION__ );
        _lock.lock();
        _cond.notify_all();
        connect_finish = true;
        _lock.unlock();
    }
    void on_close(sio::client::close_reason const& reason)
    {
        LOGI("%s: %d", __PRETTY_FUNCTION__, int(reason));
        exit(0);
    }

    void on_fail()
    {
        LOGE("%s", __PRETTY_FUNCTION__ );
        _lock.lock();
        _cond.notify_all();
        connect_finish = false;
        _lock.unlock();
    }
};

// Driver function
static void *server_thread(void *userdata)
{
    sio::client sio_client;

    connection_listener l(sio_client);

    sio_client.set_reconnect_attempts(0);
    sio_client.set_open_listener(std::bind(&connection_listener::on_connected, &l));
    sio_client.set_close_listener(std::bind(&connection_listener::on_close, &l,std::placeholders::_1));
    sio_client.set_fail_listener(std::bind(&connection_listener::on_fail, &l));

    LOGI("Tring to connect to %s:%d ...", SERVER_HOSTNAME, SERVER_PORT);
    char buf[128];
    sprintf(buf, "https://%s:%d", SERVER_HOSTNAME, SERVER_PORT);
    sio_client.connect(buf);

    _lock.lock();
    if(!connect_finish)
    {
        _cond.wait(_lock);
    }
    _lock.unlock();
    if(!connect_finish) {
        LOGE("Connection failed.");
        return nullptr;
    }

    LOGI("Connected");
    // after connection succeeded

    // emit message object with lambda ack handler

    std::string name = model_name + "-controller-" + device_name + "-00";
    std::string type = "mobile-controller";
    std::string modelName = "android-controller";

    sio::message::ptr msg = sio::object_message::create();
    msg->get_map()["name"] = sio::string_message::create(name);
    msg->get_map()["type"] = sio::string_message::create(type);
    msg->get_map()["modelName"] = sio::string_message::create(modelName);


    sio_client.socket(NAMESPACE)->emit("login", msg, [&](sio::message::list const& msg) {
        _lock.lock();
        std::map<std::string, sio::message::ptr>& res_map = msg[0]->get_map();
        res_map["ok"]->get_bool();

        for (auto const & [key, val]: res_map["data"]->get_map()) {
            if (val->get_flag() == sio::message::flag_string) {
                LOGI("%s: %s (string)", key.c_str(), val->get_string().c_str());
            } else if (val->get_flag() == sio::message::flag_integer) {
                LOGI("%s: %d (int)", key.c_str(), val->get_int());
            } else if (val->get_flag() == sio::message::flag_double) {
                LOGI("%s: %f (double)", key.c_str(), val->get_double());
            } else if (val->get_flag() == sio::message::flag_boolean) {
                LOGI("%s: %s (boolean)", key.c_str(), val->get_bool()?"true":"false");
            } else if (val->flag_integer) {
                LOGI("%s: %d (int)", key.c_str(), val->get_int());
            }
        }

        _cond.notify_all();
        _lock.unlock();
    });

    _lock.lock();
    _cond.wait(_lock);
    _lock.unlock();

    try {
        while(true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    } catch (std::exception &e) {
        LOGE("Terminated by Interrupt %s", e.what());
    }

    LOGI("Closing...");
    sio_client.sync_close();
    sio_client.clear_con_listeners();
    return nullptr;
}
