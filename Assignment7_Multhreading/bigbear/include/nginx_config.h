#ifndef NGINX_CONFIG_H
#define NGINX_CONFIG_H

#include <unordered_map>
#include <vector>
#include <string>

using std::string;

class NginxConfig
{
public:
    NginxConfig() { depth = 1; }
    explicit NginxConfig(string n) { depth = 1; name = n;  }

    std::unordered_map<string,string> getFlatParameters() const;
    std::unordered_map<string,NginxConfig*> getNestedParameters() const;

    // vector with param names in order found of context
    std::vector<string> getOrderOfParams();

    // will compare param names, returns name that was ordered first
    string compareParamOrder(string param1, string param2);

    // name of config context (will be 'server' or name of handler)
    string getName();
    unsigned getDepth();

    void addFlatParam(string key, string value);
    void addNestedParam(string handler, NginxConfig* config);

    ~NginxConfig();
private:

    // all single line params for a given context
    std::unordered_map<string, string> flatParams;
    // all nested params for a given context
    std::unordered_map<string, NginxConfig*> nestedParams;
    // stores param names in order found of context
    std::vector<string> orderOfParams;

    unsigned depth;
    string name;
};


#endif //NGINX_CONFIG_H
