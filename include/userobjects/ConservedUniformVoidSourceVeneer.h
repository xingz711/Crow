#ifndef CONSERVEDUNIFORMVOIDSOURCEEVENEER_H
#define CONSERVEDUNIFORMVOIDSOURCEEVENEER_H

/**
 * Veneer to build userobjects that generate a uniformly distributed random
 * number in the interval [0:1] once per timestep for every quadrature point
 * in a way that the integral over all random numbers is zero.
 *
 * \see ConservedUniformNoise
 * \see ConservedMaskedUniformNoise
 */
template<class T>
class ConservedUniformVoidSourceVeneer : public T
{
public:
  ConservedUniformVoidSourceVeneer(const InputParameters & parameters);

protected:
  Real getQpRandom();
};


template<class T>
ConservedUniformVoidSourceVeneer<T>::ConservedUniformVoidSourceVeneer(const InputParameters & parameters) :
    T(parameters)
{
}

template<class T>
Real
ConservedUniformVoidSourceVeneer<T>::getQpRandom()
{
  return this->getRandomReal();
}

#endif //CONSERVEDUNIFORMVOIDSOURCEEVENEER_H
