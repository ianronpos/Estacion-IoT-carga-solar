#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

class WifiManager { 
    private: 
        const char* ssid; 
        const char* password; 
        wl_status_t status; 
        
        //Event Handlers 
        WiFiEventHandler gotIpEventHandler;
        WiFiEventHandler disconneted; 

    public: 
        //Builder
        WifiManager(const char* ssid, const char* password); 

        void setup(); 
        void scan(); 
        wl_status_t getStatus(); 
        void setStatus(); 
}; 

#endif
