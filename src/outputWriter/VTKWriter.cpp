/*
 * VTKWriter.cpp
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#include "VTKWriter.h"
#include "Particle.h"

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>

namespace outputWriter {

    VTKWriter::VTKWriter() = default;

    VTKWriter::~VTKWriter() = default;

    void VTKWriter::initializeOutput(const int numParticles) {

        vtkFile = std::make_unique<VTKFile_t>("UnstructuredGrid");

        // per point, we add type, position, velocity and force
        PointData pointData;
        DataArray_t mass(type::Float32, "mass", 1);
        DataArray_t velocity(type::Float32, "velocity", 3);
        DataArray_t forces(type::Float32, "force", 3);
        DataArray_t type(type::Int32, "type", 1);
        pointData.DataArray().push_back(mass);
        pointData.DataArray().push_back(velocity);
        pointData.DataArray().push_back(forces);
        pointData.DataArray().push_back(type);

        CellData cellData; // we don't have cell data => leave it empty

        // 3 coordinates
        Points points;
        DataArray_t pointCoordinates(type::Float32, "points", 3);
        points.DataArray().push_back(pointCoordinates);

        Cells cells; // we don't have cells, => leave it empty
        // for some reasons, we have to add a dummy entry for paraview
        DataArray_t cells_data(type::Float32, "types", 0);
        cells.DataArray().push_back(cells_data);

        PieceUnstructuredGrid_t piece(pointData, cellData, points, cells, numParticles, 0);
        UnstructuredGrid_t unstructuredGrid(piece);
        vtkFile->UnstructuredGrid(unstructuredGrid);
    }

    void VTKWriter::writeFile(const std::string& filename, const int iteration) {
        std::stringstream strstr;
        strstr << filename << "_" << std::setfill('0') << std::setw(4) << iteration << ".vtu";

        std::ofstream file(strstr.str().c_str());
        VTKFile(file, *vtkFile);
        vtkFile.reset();
    }

    void VTKWriter::plotParticle(const Particle& p, const double mass) {
        if (vtkFile->UnstructuredGrid().present()) {
            SPDLOG_DEBUG("UnstructuredGrid is present");
        } else {
            SPDLOG_ERROR("ERROR: No UnstructuredGrid present");
        }

        PointData::DataArray_sequence& pointDataSequence = vtkFile->UnstructuredGrid()->Piece().PointData().DataArray();
        PointData::DataArray_iterator dataIterator = pointDataSequence.begin();

        dataIterator->push_back(mass);
        // cout << "Appended mass data in: " << dataIterator->Name();

        dataIterator++;
        dataIterator->push_back(p.getV()[0]);
        dataIterator->push_back(p.getV()[1]);
        dataIterator->push_back(p.getV()[2]);
        // cout << "Appended velocity data in: " << dataIterator->Name();

        dataIterator++;
        dataIterator->push_back(p.getOldF()[0]);
        dataIterator->push_back(p.getOldF()[1]);
        dataIterator->push_back(p.getOldF()[2]);
        // cout << "Appended force data in: " << dataIterator->Name();

        dataIterator++;
        dataIterator->push_back(p.getType());

        Points::DataArray_sequence& pointsSequence = vtkFile->UnstructuredGrid()->Piece().Points().DataArray();
        Points::DataArray_iterator pointsIterator = pointsSequence.begin();
        pointsIterator->push_back(p.getX()[0]);
        pointsIterator->push_back(p.getX()[1]);
        pointsIterator->push_back(p.getX()[2]);
    }

    void VTKWriter::plotParticles(const ParticleContainer& container, const std::string& filename, const int iteration) {
        initializeOutput(container.size());

        for (const Particle& p : container) {
            this->plotParticle(p, container.get_type_descriptor(p.getType()).get_mass());
        }

        writeFile(filename, iteration);
    }

} // namespace outputWriter
