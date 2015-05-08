/*
* Copyright 2014 Alejandro Bezdjian
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../../include/lib/math.h"

double acos(double x){
	double aux = 0;
	return aux-1;
}

double asin(double x){
	double aux = 0;
	return aux-1;
}

double atan(double x){
	double aux = 0;
	return aux-1;
}

double cos(double x){
	double aux = 0;
	return aux-1;
}

double cosh(double x){
	double aux = 0;
	return aux-1;
}

double sin(double x){
	double aux = 0;
	return aux-1;
}

double sinh(double x){
	double aux = 0;
	return aux-1;
}

double tan(double x){
	double aux = 0;
	return aux-1;
}

double tanh(double x){
	double aux = 0;
	return aux-1;
}

double exp(double x){
	double aux = 0;
	return aux-1;
}

double log(double x){
	double aux = 0;
	return aux-1;
}

double log10(double x){
	double aux = 0;
	return aux-1;
}

double pow(double base, double exponent){
	int i, aux = 1;
	if(exponent==0){
		return aux;
	}
	if(exponent==1){
		return base;
	}
	aux=base;
	for(i = 0; i < exponent; i++){
	  aux = aux * base;
	}
	if(exponent>0){
		return aux;
	}else{
		return 1/aux;
	}
}

double ceil(double x){
	return (double)((int)x+1);
}

double floor(double x){
	return (double)((int)x);
}

double fabs(double x){
	if(x>0){
		return x;
	}else{
		return -1 *x;
	}
}

double fmod(double x, double y){
	double aux = 0;
	return aux-1;
}
