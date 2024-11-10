#include "ParticleContainer.h"

ParticleContainer::ParticleContainer() = default;

ParticleContainer::ParticleContainer(const std::vector<Particle>& new_particles) { particles = new_particles; }

ParticleContainer::~ParticleContainer() = default;

std::vector<Particle>& ParticleContainer::get_particles() { return begin(); }

class PIterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = Particle;
    using pointer = Particle*;
    using reference = Particle&;

    PIterator(pointer ptr)
        : m_ptr(ptr) { }

    reference operator*() /*const?*/ { return *m_ptr; }
    pointer operator->() /*const?*/ { return m_ptr; }

    // Prefix/Postfix increment
    PIterator& operator++() {
        ++m_ptr;
        return *this;
    }
    PIterator operator++(int) {
        PIterator tmp = *this;
        ++(*this);
        return tmp;
    }
    // Prefix/Postfix decrement
    PIterator& operator--() {
        --m_ptr;
        return *this;
    }
    PIterator operator--(int) {
        PIterator tmp = *this;
        --(*this);
        return tmp;
    }

    difference_type operator-(const PIterator& other) const { return m_ptr - other.m_ptr; }

    PIterator operator+(difference_type offset) const { return PIterator(m_ptr + offset); }
    PIterator operator-(difference_type offset) const { return PIterator(m_ptr - offset); }
    PIterator& operator+=(difference_type offset) {
        m_ptr += offset;
        return *this;
    }
    PIterator& operator-=(difference_type offset) {
        m_ptr -= offset;
        return *this;
    }

    reference operator[](difference_type diff) const { return m_ptr[diff]; }

    bool operator==(const PIterator& other) const { return m_ptr == other.m_ptr; }
    bool operator!=(const PIterator& other) const { return m_ptr != other.m_ptr; }
    bool operator<(const PIterator& other) const { return m_ptr < other.m_ptr; }
    bool operator>(const PIterator& other) const { return m_ptr > other.m_ptr; }
    bool operator<=(const PIterator& other) const { return m_ptr <= other.m_ptr; }
    bool operator>=(const PIterator& other) const { return m_ptr >= other.m_ptr; }

private:
    pointer m_ptr;
};

PIterator begin() { return PIterator(particles); }
PIterator end() {return PIterator(particles + particles.size())}