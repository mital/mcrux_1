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

#include "MCruxWindowConfiguration.h"


MCruxWindowConfiguration::MCruxWindowConfiguration(const wstring _windowTitle,
												   unsigned int _width,
												   unsigned int _height,
												   const wstring & _url)
: windowTitle(_windowTitle),
  width(_width),
  height(_height),
  url(_url)
{
}

MCruxWindowConfiguration::~MCruxWindowConfiguration()
{
}


wstring MCruxWindowConfiguration::getWindowTitle() const
{
	return windowTitle;
}

wstring MCruxWindowConfiguration::getURL() const
{
	return url;
}

unsigned int MCruxWindowConfiguration::getWidth() const
{
	return width;
}

unsigned int MCruxWindowConfiguration::getHeight() const
{
	return height;
}