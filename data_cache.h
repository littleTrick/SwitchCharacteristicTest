#ifndef DATA_CACHE_H
#define DATA_CACHE_H

#include <QVector>

class DataCache
{
public:
    DataCache();
    void PushDataToAfracture(unsigned char data);
    void PushDataToBfracture(unsigned char data);
    void PushDataToCfracture(unsigned char data);
    void PushDataToDisplacement(unsigned char data);
    void PushDataToCoilCurrent(unsigned char data);

    QVector<char> &PopDataFromAfracture();
    QVector<char> &PopDataFromBfracture();
    QVector<char> &PopDataFromCfracture();
    QVector<char> &PopDataFromDisplacement();
    QVector<char> &PopDataFromCoilCurrent();

    unsigned int AfractureSize();
    unsigned int BfractureSize();
    unsigned int CfractureSize();
    unsigned int DisplacementSize();
    unsigned int CoilCurrentSize();

private:
    QVector<double> data_to_draw;
    QVector<char> _data_A_fracture;
    QVector<char> _data_B_fracture;
    QVector<char> _data_C_fracture;
    QVector<char> _data_displacement;
    QVector<char> _data_coil_current;
};

#endif // DATA_CACHE_H
