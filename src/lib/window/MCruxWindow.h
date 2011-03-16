/**
 * copyright (C) 2009 Mital Vora. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of MCrux nor the names of its contributors may be 
 *     used to endorse or promote products derived from this software 
 *     without specific prior written permission.
 *
 * @author: Mital Vora.
 **/

#ifndef _MCRUXWINDOW_H_
#define _MCRUXWINDOW_H_

#include <map>
#include <iostream>

using namespace std;

#include "MCruxWindowConfiguration.h"

class MCruxWindow
{
protected:
	const MCruxWindowConfiguration * config;

public:
	MCruxWindow(const MCruxWindowConfiguration * _config);
	virtual ~MCruxWindow();

	virtual int ShowWindow() const=0;
	virtual int HideWindow() const=0;
	virtual int UpdateWindow() const=0;
	virtual void resize() const=0;
};

#endif // _MCRUXWINDOW_H_