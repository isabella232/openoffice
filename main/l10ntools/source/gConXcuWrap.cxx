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
#include "gConXcu.hxx"
#include <iostream>
#include <fstream>
#include <cstdlib>


/*****************************************************************************
 *********************   G C O N X C U W R A P . C X X   *********************
 *****************************************************************************
 * This includes the c code generated by flex
 *****************************************************************************/



/************   I N T E R F A C E   I M P L E M E N T A T I O N   ************/
convert_xcu::convert_xcu(l10nMem& crMemory)
                        : convert_gen_impl(crMemory),
                          mbNoCollectingData(true)
{
}



/************   I N T E R F A C E   I M P L E M E N T A T I O N   ************/
convert_xcu::~convert_xcu()
{
}



/**********************   I M P L E M E N T A T I O N   **********************/
namespace XcuWrap
{
#define IMPLptr convert_gen_impl::mcImpl
#define LOCptr ((convert_xcu *)convert_gen_impl::mcImpl)
#include "gConXcu_yy.c"
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_xcu::execute()
{
  XcuWrap::yylex();
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_xcu::pushKey(char *syyText, bool bIsComponent)
{
  std::string sKey, sTag = copySource(syyText);
  int    nL, nE;

  // find key in tag
  nL = sTag.find("oor:name=\"");
  if (nL != (int)std::string::npos)
  {
    // find end of key
    nL += 10;
    nE  = sTag.find("\"", nL);
    if (nE != (int)std::string::npos)
      sKey = (bIsComponent ? "." : "") + sTag.substr(nL, nE - nL);
  }
  mcStack.push_back(sKey);
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_xcu::popKey(char *syyText)
{
  copySource(syyText);

  // check for correct node/prop relations
  if (mcStack.size())
    mcStack.pop_back();
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_xcu::startCollectData(char *syyText)
{
  int nL;
  std::string sTag = copySource(syyText);

  // locate object name
  nL = sTag.find("xml:lang=\"");
  if (nL != (int)std::string::npos)
  {
    // test langauge
    nL += 10;
    if (sTag.substr(nL,5) != "en-US")
    {
      std::string sErr = sTag.substr(nL,5) + " is not en-US";
      showError((char *)sErr.c_str());
    }
    else
      mbNoCollectingData = false;
  }
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_xcu::stopCollectData(char *syyText)
{
  int    nL;
  std::string useKey, useText = msCollector;
  
  copySource(syyText);

  // time to do something ?
  if (mbNoCollectingData)
    return;

  mbNoCollectingData = true;

  // locate key and extract it
  for (nL = 0; nL < (int)mcStack.size(); ++nL)
    useKey += (useKey.size() ? "." : "" ) + mcStack[nL];
  
  if (mbMergeMode)
  {
    // get all languages (includes en-US)
    std::vector<l10nMem_entry *>& cExtraLangauges = mcMemory.getLanguagesForKey(useKey);
    std::string                   sNewLine;
    nL = cExtraLangauges.size();

    for (int i = 0; i < nL; ++i)
    {
      sNewLine = "<value xml:lang=\"" + cExtraLangauges[i]->msLanguage + "\">" +
               cExtraLangauges[i]->msText + "</value>";
      writeSourceFile(sNewLine);
    }
  }
  else
    mcMemory.setEnUsKey(useKey, "value", useText);
}  
