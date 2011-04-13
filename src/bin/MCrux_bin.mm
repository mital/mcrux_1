//
//  main.m
//  MCrux_bin
//
//  Created by Mital Vora on 09/04/11.
//  Copyright 2011 MCrux Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>


#include <mcrux/MCrux.h>


std::string getPath()
{
	NSString* bundlePath = [[NSBundle mainBundle] bundlePath];
	return std::string([bundlePath UTF8String]);
	
}

int main(int argc, char *argv[])
{	
	const std::string path(getPath());
	std::string mcruxspec_path(path);
	mcruxspec_path += "\\app.mcruxspec";
	std::string plugins_path(path);
	plugins_path += "\\plugins";
	
	MCrux mcrux;
	mcrux.InitializeAndRunWith(mcruxspec_path, plugins_path, argc, argv);
    return 0;
}
