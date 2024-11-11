#include "ParticleContainer.h"

ParticleContainer::ParticleContainer() = default;

ParticleContainer::ParticleContainer(const std::vector<Particle>& new_particles) { particles = new_particles; }

ParticleContainer::~ParticleContainer() = default;

std::vector<Particle> ParticleContainer::get_particles() { return particles; }

ParticleContainer::PIterator::PIterator(ParticleContainer::PIterator::pointer ptr)
    : m_ptr(ptr) { }

ParticleContainer::PIterator::reference ParticleContainer::PIterator::operator*() { return *m_ptr; }
ParticleContainer::PIterator::pointer ParticleContainer::PIterator::operator->() /*const?*/ { return m_ptr; }

// Prefix/Postfix increment
ParticleContainer::PIterator& ParticleContainer::PIterator::operator++() {
    ++m_ptr;
    return *this;
}
ParticleContainer::PIterator ParticleContainer::PIterator::operator++(int) {
    PIterator tmp = *this;
    ++(*this);
    return tmp;
}

// Prefix/Postfix decrement
ParticleContainer::PIterator& ParticleContainer::PIterator::operator--() {
    --m_ptr;
    return *this;
}
ParticleContainer::PIterator ParticleContainer::PIterator::operator--(int) {
    PIterator tmp = *this;
    --(*this);
    return tmp;
}

ParticleContainer::PIterator::difference_type ParticleContainer::PIterator::operator-(const ParticleContainer::PIterator& other) const {
    return m_ptr - other.m_ptr;
}

ParticleContainer::PIterator ParticleContainer::PIterator::operator+(ParticleContainer::PIterator::difference_type offset) const {
    return PIterator(m_ptr + offset);
}
ParticleContainer::PIterator ParticleContainer::PIterator::operator-(ParticleContainer::PIterator::difference_type offset) const {
    return PIterator(m_ptr - offset);
}
ParticleContainer::PIterator& ParticleContainer::PIterator::operator+=(ParticleContainer::PIterator::difference_type offset) {
    m_ptr += offset;
    return *this;
}
ParticleContainer::PIterator& ParticleContainer::PIterator::operator-=(ParticleContainer::PIterator::difference_type offset) {
    m_ptr -= offset;
    return *this;
}

ParticleContainer::PIterator::reference ParticleContainer::PIterator::operator[](ParticleContainer::PIterator::difference_type diff) const {
    return m_ptr[diff];
}

bool ParticleContainer::PIterator::operator==(const PIterator& other) const { return m_ptr == other.m_ptr; }
bool ParticleContainer::PIterator::operator!=(const PIterator& other) const { return m_ptr != other.m_ptr; }
bool ParticleContainer::PIterator::operator<(const PIterator& other) const { return m_ptr < other.m_ptr; }
bool ParticleContainer::PIterator::operator>(const PIterator& other) const { return m_ptr > other.m_ptr; }
bool ParticleContainer::PIterator::operator<=(const PIterator& other) const { return m_ptr <= other.m_ptr; }
bool ParticleContainer::PIterator::operator>=(const PIterator& other) const { return m_ptr >= other.m_ptr; }

// private:
//     pointer m_ptr;
// };

// PIterator begin() { return PIterator(particles); }
// PIterator end() { return PIterator(particles + particles.size()) }
