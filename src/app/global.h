/*
 *	Dualword-pmc is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Dualword-pmc is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with Dualword-pmc. If not, see <http://www.gnu.org/licenses/>.
 *
*/

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "app/DualwordPmcApp.h"
#include "gui/MainWindow.h"
#include "app/FormDbConnection.h"
#include "app/Doc.h"

#if defined(pmcApp)
#undef pmcApp
#endif
#define pmcApp (DualwordPmcApp::instance())

#if defined(mainWin)
#undef mainWin
#endif
#define mainWin (DualwordPmcApp::instance()->window())

#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <exception>
#include <string>

class dualword_exception: public std::runtime_error {
public:
	dualword_exception(const std::string& arg) : runtime_error(arg) {}
};

#endif /* GLOBAL_H_ */
