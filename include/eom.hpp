#ifndef EOM_HPP
#define EOM_HPP

double g(double distance, double body_length){
    return distance <= body_length ? body_length/distance : 1.;
}

#endif // EOM_HPP