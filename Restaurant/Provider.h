#ifndef PROVIDER_H
#define PROVIDER_H

#include <map>
#include <string>

class Provider {
public:
    static void ProcessOrder(const std::map<std::string, int>& order, double& balance);
    std::map<std::string, int> ViewOrder(const std::string& orderFilename);
    void RunProvider();
};

#endif
