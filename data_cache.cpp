#include "data_cache.h"

DataCache::DataCache()
{
}

void DataCache::PushDataToAfracture(unsigned char data)
{
    _data_A_fracture.push_back(data);
}

void DataCache::PushDataToBfracture(unsigned char data)
{
    _data_B_fracture.push_back(data);
}

void DataCache::PushDataToCfracture(unsigned char data)
{
    _data_C_fracture.push_back(data);
}

void DataCache::PushDataToDisplacement(unsigned char data)
{
    _data_displacement.push_back(data);
}

void DataCache::PushDataToCoilCurrent(unsigned char data)
{
    _data_coil_current.push_back(data);
}

QVector<char> &DataCache::PopDataFromAfracture()
{
    return _data_A_fracture;
}

QVector<char> &DataCache::PopDataFromBfracture()
{
    return _data_B_fracture;
}

QVector<char> &DataCache::PopDataFromCfracture()
{
    return _data_C_fracture;
}

QVector<char> &DataCache::PopDataFromDisplacement()
{
    return _data_displacement;
}

QVector<char> &DataCache::PopDataFromCoilCurrent()
{
    return _data_coil_current;
}
unsigned int DataCache::AfractureSize()
{
    return _data_A_fracture.size();
}

unsigned int DataCache::BfractureSize()
{
    return _data_B_fracture.size();
}

unsigned int DataCache::CfractureSize()
{
    return _data_C_fracture.size();
}

unsigned int DataCache::DisplacementSize()
{
    return _data_displacement.size();
}

unsigned int DataCache::CoilCurrentSize()
{
    return _data_coil_current.size();
}
