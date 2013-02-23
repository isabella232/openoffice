/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/
#include "gConSrc.hxx"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <string.h>


/*****************************************************************************
 *********************   G C O N S R C W R A P . C X X   *********************
 *****************************************************************************
 * This includes the c code generated by flex
 *****************************************************************************/



/************   I N T E R F A C E   I M P L E M E N T A T I O N   ************/
convert_src::convert_src(l10nMem& crMemory)
                        : convert_gen_impl(crMemory),
                          mbEnUs(false),
                          mbExpectName(false),
                          mbExpectMacro(false),
                          mbExpectStringList(false),
                          mbExpectValue(false),
                          mbAutoPush(false),
                          mbValuePresent(false)
{}
convert_src::~convert_src()
{}



/**********************   I M P L E M E N T A T I O N   **********************/
namespace SrcWrap
{
#define IMPLptr convert_gen_impl::mcImpl
#define LOCptr ((convert_src *)convert_gen_impl::mcImpl)
#include "gConSrc_yy.c"
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_src::execute()
{
  SrcWrap::yylex();
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_src::setValue(char *syyText, char *sbuildValue)
{
  if (mbExpectStringList)
  {
    std::stringstream ssBuf;

    msTextName   = msSaveTextName;
    mbEnUs       = 
    mbExpectName = true;
    ssBuf        << ++miListCount;
    msName       = ssBuf.str();
    mcStack.pop_back();
    mcStack.push_back(msName);
  }

  copySource(syyText);
  msValue        = sbuildValue;
  mbValuePresent = true;
  mbExpectValue  = false;
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_src::setLang(char *syyText, bool bEnUs)
{
  std::string useText = copySource(syyText) + " is no en-US language";

  mbEnUs = bEnUs;
  if (!bEnUs && mbExpectValue)
    showError((char *)useText.c_str());
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_src::setId(char *syyText, bool bId)
{
  copySource(syyText);
  if (bId || !mcStack.back().size())
    mbExpectName = mbAutoPush = true;
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_src::setText(char *syyText)
{
  msTextName    = copySource(syyText);
  mbExpectValue = true;
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_src::setName(char *syyText)
{
  std::string useText = copySource(syyText);

  if (mbExpectName)
  {
    mbExpectName = false;
    if (!mbAutoPush)
      msName = useText;
    else
    {
      mbAutoPush = false;
      if (mcStack.size())
        mcStack.pop_back();
      mcStack.push_back(useText);
    }
  }
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_src::setCmd(char *syyText)
{
  copySource(syyText);
  mbExpectName = true;
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_src::setMacro(char *syyText)
{
  copySource(syyText);
  mbExpectName  =
  mbExpectMacro =
  mbAutoPush    = true;
  mcStack.push_back("");
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_src::setList(char *syyText)
{
  msSaveTextName = msTextName  = copySource(syyText);
  miListCount    = 0;
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_src::setStringList(char *syyText)
{
  msSaveTextName = msTextName  = copySource(syyText);
  mbExpectStringList = true;

  miListCount        = 0;
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_src::setNL(char *syyText, bool bMacro)
{
  int         nL;
  std::string sKey;


  copySource(syyText);

  if (msTextName.size() && mbValuePresent && mbEnUs)
  {
    // locate key and extract it
    sKey.clear();
    for (nL = 0; nL < (int)mcStack.size(); ++nL)
      if (mcStack[nL].size())
        sKey += (sKey.size() ? "." : "") + mcStack[nL];

    mcMemory.setEnUsKey(sKey, msTextName, msValue);
  }

  if (!bMacro && mbExpectMacro)
  {
    if (mcStack.size())
      mcStack.pop_back();
    mbEnUs        =
    mbExpectMacro = false;
  }

  mbValuePresent =
  mbExpectName   =
  mbAutoPush     = false;
  msValue.clear();
  msTextName.clear();
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_src::startBlock(char *syyText)
{
  copySource(syyText);

  mcStack.push_back(msName);
  msName.clear();
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_src::stopBlock(char *syyText)
{
  copySource(syyText);

  // check for correct node/prop relations
  if (mcStack.size())
    mcStack.pop_back();
  mbExpectStringList = false;
  mbEnUs             = false;
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_src::setListItem(char *syyText, bool bIsStart)
{
  copySource(syyText);
  mbExpectStringList = false;

  if (bIsStart)
  {
    msTextName   = msSaveTextName;
    mbExpectValue = 
    mbExpectName  = true;
    msName.clear();
  }
  else
  {
    std::stringstream ssBuf;

    mbExpectName = false;
    mcStack.pop_back();
    ssBuf << ++miListCount;
//    if (msName.size())
//      ssBuf << "." << msName;
    msName = ssBuf.str();
    mcStack.push_back(msName);
  }
}
