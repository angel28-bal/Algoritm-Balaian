#pragma once

#include <iostream>
using namespace std;

template <typename T> 
T GetCorrectNumber(T min, T max){
	T x;
	while (!(cin >> x) || cin.fail() || (std::cin.peek() != '\n') || (x < min) || (x > max)){
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Введите корректные данные (число в пределах: " << min << "-" << max << "):";
	}
	return x;
}