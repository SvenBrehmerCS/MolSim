/*
 * Particle.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "Particle.h"

#include "utils/Vec.h"
#include <iostream>
#include <spdlog/spdlog.h>

Particle::Particle(const int type_arg) {
    type = type_arg;
    SPDLOG_TRACE("Particle generated without args!");
    f = { 0., 0., 0. };
    old_f = { 0., 0., 0. };
}

Particle::Particle(const Particle& other) {
    x = other.x;
    v = other.v;
    f = other.f;
    old_f = other.old_f;
    type = other.type;
    neighbors = other.neighbors;
    index = other.index;
    in_molecule = other.in_molecule;

    SPDLOG_TRACE("Particle generated by copy!");
}

Particle::Particle(const Vec<double>& x_arg, const Vec<double>& v_arg, const int type_arg) {
    x = x_arg;
    v = v_arg;
    type = type_arg;
    f = { 0., 0., 0. };
    old_f = { 0., 0., 0. };
    SPDLOG_TRACE("Particle generated with args!");
}

Particle::~Particle() { SPDLOG_TRACE("Particle destructed!"); }

const Vec<double>& Particle::getX() const { return x; }

const Vec<double>& Particle::getV() const { return v; }

const Vec<double>& Particle::getF() const { return f; }

const Vec<double>& Particle::getOldF() const { return old_f; }

int Particle::getType() const { return type; }

size_t Particle::getIndex() const { return index; }

bool Particle::isMolecule() const { return in_molecule; }

bool Particle::isDirectNeighbour(const size_t idx) const {
    return (idx == neighbors[0]) | (idx == neighbors[1]) | (idx == neighbors[2]) | (idx == neighbors[3]);
}

bool Particle::isCornerNeighbour(const size_t idx) const {
    return (idx == neighbors[4]) | (idx == neighbors[5]) | (idx == neighbors[6]) | (idx == neighbors[7]);
}

void Particle::setIndex(size_t idx) { index = idx; }

void Particle::setNeighbors(const std::array<size_t, 8>& new_neighbors) { neighbors = new_neighbors; }

void Particle::setInMolecule(bool in_molecule_arg) { in_molecule = in_molecule_arg; }

void Particle::setX(const Vec<double>& x_new) { x = x_new; }

void Particle::setV(const Vec<double>& v_new) { v = v_new; }

void Particle::setF(const Vec<double>& f_new) { f = f_new; }

void Particle::setOldF(const Vec<double>& old_f_new) { old_f = old_f_new; }

void Particle::setType(int new_type) { type = new_type; }

std::string Particle::toString() const {
    std::stringstream stream;
    stream << "Particle: X: " << x << " v: " << v << " f: " << f << " old_f: " << old_f << " type: " << type;
    return stream.str();
}

bool Particle::operator==(const Particle& other) const {
    return (x == other.getX()) && (v == other.getV()) && (f == other.getF()) && (old_f == other.getOldF() && (type == other.getType()));
}

std::ostream& operator<<(std::ostream& stream, const Particle& p) {
    stream << p.toString();
    return stream;
}
