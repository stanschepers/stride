/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2019, Jan Broeckhove and Bistromatics group.
 */

#ifndef STRIDE_MAPCONTROLLER_H
#define STRIDE_MAPCONTROLLER_H

#include <QObject>
#include <QString>

#include "EpiOutput.h"

namespace visualization {

class MapController : public QObject
{

        Q_OBJECT
        Q_PROPERTY(QString setDay WRITE setDay)
        Q_PROPERTY(QString setWindowHeight WRITE setWindowHeight)
        Q_PROPERTY(QString setWindowWidth WRITE setWindowWidth)

public:
        MapController() = default;

        /// Create a MapController and read the epi-output file
        explicit MapController(const std::string& filename);

        /// Set the shown day
        void setDay(const QString& day);

        /// Set the window height
        void setWindowHeight(const QString& height);

        /// Set the window width
        void setWindowWidth(const QString& width);

        /// Initialize the map
        void initialize(QObject* root);

private:
        void update() const;

        EpiOutput m_epiOutput;

        unsigned int m_day = 0;      // Current shown step
        unsigned int m_day_diff = 0; // Difference between epi-output measurements (Amount of days)
        unsigned int m_window_height = 0;
        unsigned int m_window_width = 0;

        QObject* m_root;
};

} // namespace visualization

#endif // STRIDE_MAPCONTROLLER_H
