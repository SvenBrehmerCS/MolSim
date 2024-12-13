/*
 * Particle.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "Particle.h"

#include "utils/ArrayUtils.h"
#include <iostream>
#include <spdlog/spdlog.h>

Particle::Particle(int type_arg) {
    type = type_arg;
    spdlog::trace("Particle generated without args!");
    f = { 0., 0., 0. };
    old_f = { 0., 0., 0. };
}

Particle::Particle(const Particle& other) {
    x = other.x;
    v = other.v;
    f = other.f;
    old_f = other.old_f;
    type = other.type;
    spdlog::trace("Particle generated by copy!");
}

Particle::Particle(const std::array<double, 3>& x_arg, const std::array<double, 3>& v_arg, const int type_arg) {
    x = x_arg;
    v = v_arg;
    type = type_arg;
    f = { 0., 0., 0. };
    old_f = { 0., 0., 0. };
    spdlog::trace("Particle generated with args!");
}

Particle::~Particle() { spdlog::trace("Particle destructed!"); }

const std::array<double, 3>& Particle::getX() const { return x; }

const std::array<double, 3>& Particle::getV() const { return v; }

const std::array<double, 3>& Particle::getF() const { return f; }

const std::array<double, 3>& Particle::getOldF() const { return old_f; }

int Particle::getType() const { return type; }

void Particle::setX(const std::array<double, 3>& x_new) { x = x_new; }

void Particle::setV(const std::array<double, 3>& v_new) { v = v_new; }

void Particle::setF(const std::array<double, 3>& f_new) { f = f_new; }

void Particle::setOldF(const std::array<double, 3>& old_f_new) { old_f = old_f_new; }

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
