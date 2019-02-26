#include "nginx_config.h"

std::unordered_map<string, string> NginxConfig::getFlatParameters() const
{
    return flatParams;
}

std::unordered_map<string, NginxConfig *> NginxConfig::getNestedParameters() const
{
    return nestedParams;
}

std::vector<string> NginxConfig::getOrderOfParams()
{
    return orderOfParams;
}

string NginxConfig::getName()
{
    return name;
}

unsigned NginxConfig::getDepth()
{
    return depth;
}

void NginxConfig::addFlatParam(string key, string value)
{
    auto it = flatParams.find(key);
    if(it != flatParams.end()) it-> second = value;
    else flatParams.insert( {key, value} );
    orderOfParams.push_back(key);
}

void NginxConfig::addNestedParam(string handler, NginxConfig* config)
{
    nestedParams.insert( {handler, config} );
    orderOfParams.push_back(handler);
    depth++;
}

string NginxConfig::compareParamOrder(string param1, string param2)
{
    for(auto name : orderOfParams)
    {
        if(name == param1)
        {
            return param1;
        }
        else if(name == param2)
        {
            return param2;
        }
    }
    // neither param was found in config
    return "";
}

NginxConfig::~NginxConfig()
{
    std::unordered_map<string, NginxConfig*>::iterator it;
    for(it = nestedParams.begin(); it != nestedParams.end(); it++)
    {
        // deletes each nested config
        delete it->second;
    }
}
