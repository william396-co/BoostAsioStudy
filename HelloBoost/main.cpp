#include <iostream>
#include <boost/version.hpp>
#include <boost/config.hpp>

using std::cout;
int main() {

	cout << BOOST_VERSION << "\n"
		<< BOOST_LIB_VERSION << "\n"
		<< BOOST_PLATFORM << "\n"
		<< BOOST_COMPILER << "\n"
		<< BOOST_STDLIB << "\n";

	system("Pause");


	return 0;
}