//
// Created by laurens on 4/9/19.
//

#ifndef STRIDE_MAPCONTROLLER_H
#define STRIDE_MAPCONTROLLER_H

#include <QObject>

#include "EpiOutput.h"

namespace visualization {

class MapController: public QObject {


public:
    MapController() = default;

    explicit MapController(const std::string& filename);

    void setDay(unsigned int day);

private:
    EpiOutput m_epiOutput;

    unsigned int m_day;

};

}

#endif //STRIDE_MAPCONTROLLER_H
