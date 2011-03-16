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

#include "MCruxSpecParser.h"
#include <algorithm>

#ifndef CW_USEDEFAULT
//todo: find proper default height width
#define CW_USEDEFAULT 500
#endif

bool XMLParser::getProperties(_xmlAttr * properties, map<wstring, wstring> & attrMap)
{
	for(_xmlAttr * childprop = properties;
		childprop;
		childprop = childprop->next)
	{
		
		for(xmlNode * child = childprop->children;
			child;
			child = child->next)
		{
			string name = (char*)childprop->name;
			string content = (char*)childprop->children->content;
			wstring wname(name.begin(), name.end());
			wstring wcontent(content.begin(), content.end());
			attrMap[wname] = wcontent;
		}
	}
	return true;
}


MCruxSpecParser::MCruxSpecParser()
{
}

MCruxSpecParser::~MCruxSpecParser()
{
	xmlCleanupParser();
}

wstring MCruxSpecParser::getURL(xmlNode *child_prop) const
{
	xmlChar * text = xmlCharStrdup("text");
	if (0 == xmlStrcmp(child_prop->children->name, text))
	{
		string url8 = (char*)child_prop->children->content;
		wstring url(url8.begin(), url8.end());
		return url;
	}
	wstring url;
	return url;
}


bool MCruxSpecParser::parseWindowElement(xmlNode *windowNode)
{
	wstring windowTitle;
	wstring url;
	unsigned int width = CW_USEDEFAULT;
	unsigned int height = CW_USEDEFAULT;

	//xmlChar * windowTitleName = xmlCharStrdup(MCRUXSPEC_WINDOW_TITLE_NAME);
	xmlChar * urlName = xmlCharStrdup(MCRUXSPEC_WINDOW_URL_NAME);

	map<wstring, wstring> attrlist;
	XMLParser::getProperties(windowNode->properties, attrlist);
	windowTitle = attrlist[L"title"];

	wstring wwidthstr = attrlist[L"width"];
	string widthstr(wwidthstr.begin(), wwidthstr.end());
	width = atoi(widthstr.c_str());

	wstring wheightstr = attrlist[L"height"];
	string heightstr(wheightstr.begin(), wheightstr.end());
	height = atoi(heightstr.c_str());

	for(xmlNode * child_prop = windowNode->children;
		child_prop;
		child_prop = child_prop->next)
	{
		if (0 == xmlStrcmp(child_prop->name, urlName))
		{
			url = getURL(child_prop);
		}
	}

	MCruxWindowConfiguration * winConf
		= new MCruxWindowConfiguration(windowTitle,
		width,
		height,
		url);
	windowConfigs.push_back(winConf);
	return true;
}

bool MCruxSpecParser::parseWindowsElement(xmlNode *windowsNode)
{
	const xmlChar * windowName = xmlCharStrdup(MCRUXSPEC_WINDOW_NAME);
	for(xmlNode * child_prop = windowsNode->children;
		child_prop;
		child_prop = child_prop->next)
	{
		if (0 == xmlStrcmp(child_prop->name, windowName))
		{
			parseWindowElement(child_prop);
		}
	}
	return true;
}

bool MCruxSpecParser::parsePluginsElement(xmlNode *pluginsNode)
{
	xmlChar * pluginName = xmlCharStrdup(MCRUXSPEC_PLUGIN_NAME);
	for(xmlNode * child_prop = pluginsNode->children;
		child_prop;
		child_prop = child_prop->next)
	{
		if (0 == xmlStrcmp(child_prop->name, pluginName))
		{
			map<wstring, wstring> attrlist;
			XMLParser::getProperties(child_prop->properties, attrlist);
			plugins.push_back(attrlist[L"name"]);
		}
	}
	return true;
}

bool MCruxSpecParser::parseMCruxSpecXMLNSVersion1(xmlNode *root_child)
{
	const xmlChar * mCruxSpecWindowsName = xmlCharStrdup(MCRUXSPEC_WINDOWS_NAME);
	const xmlChar * mCruxSpecPluginsName = xmlCharStrdup(MCRUXSPEC_PLUGINS_NAME);

	for (xmlNode *cur_node = root_child;
		cur_node != NULL;
		cur_node = cur_node->next)
	{
		if (cur_node->type == XML_ELEMENT_NODE)
		{
			if (0 == xmlStrcmp(cur_node->name, mCruxSpecWindowsName))
			{
				parseWindowsElement(cur_node);
			}
			else if (0 == xmlStrcmp(cur_node->name, mCruxSpecPluginsName))
			{
				parsePluginsElement(cur_node);
			}
		}
		
	}

	return true;
}

bool MCruxSpecParser::parseMCruxSpecRootElement(xmlNode *root_element)
{
	if (root_element)
	{
		xmlChar * mCruxSpecRootName = xmlCharStrdup(MCRUXSPEC_ROOT_NAME);

		if ((root_element->type == XML_ELEMENT_NODE)
			&& (0 == xmlStrcmp(root_element->name, mCruxSpecRootName)))
		{
			xmlChar * mCruxSpecNS = xmlCharStrdup(MCRUXSPEC_XMLNS_VERSION_1);
			if ((root_element->ns)
				&& (root_element->ns->type == XML_NAMESPACE_DECL)
				&& (0 == xmlStrcmp(mCruxSpecNS, root_element->ns->href)))
			{
				return parseMCruxSpecXMLNSVersion1(root_element->children);
			}
		}
	}
	return false;
}


bool MCruxSpecParser::parse(const string & mcruxAppConfigFileName)
{
	xmlDoc *doc = xmlReadFile(mcruxAppConfigFileName.c_str(), NULL, 0);;

	if (doc != NULL)
	{
		parseMCruxSpecRootElement(xmlDocGetRootElement(doc));

		xmlFreeDoc(doc);
		return true;
	}
	return false;
}

bool MCruxSpecParser::getWindowConfigList(list <MCruxWindowConfiguration *> & mcruxWindowConfigs) const
{
	if(windowConfigs.size())
	{
		for(list <MCruxWindowConfiguration *>::const_iterator
			oIter = windowConfigs.begin();
			oIter != windowConfigs.end();
		oIter++)
		{
			mcruxWindowConfigs.push_back(*oIter);
		}
		return true;
	}
	return false;
	
}

bool MCruxSpecParser::getPlugins(list <wstring> & _plugins) const
{
	if(plugins.size())
	{
		for(list <wstring>::const_iterator
			oIter = plugins.begin();
			oIter != plugins.end();
		oIter++)
		{
			_plugins.push_back(*oIter);
		}
		return true;
	}
	return false;
}
