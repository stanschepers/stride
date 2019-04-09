//
// Created by laurens on 4/9/19.
//

#include "MapController.h"
#include "EpiOutputReaderFactory.h"

namespace visualization {

MapController::MapController(const std::string &filename){
    visualization::EpiOutputReaderFactory readerFactory;
    const auto& reader = readerFactory.CreateReader(filename, m_epiOutput);
    reader->Read();
    m_day = m_epiOutput.begin()->second.begin()->second.begin()->second.begin()->first;
}

void MapController::setDay(unsigned int day) {
    m_day = day;
}

}