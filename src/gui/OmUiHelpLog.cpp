/*
  This file is part of Open Mod Manager.

  Open Mod Manager is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Open Mod Manager is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Open Mod Manager. If not, see <http://www.gnu.org/licenses/>.
*/

#include "gui/res/resource.h"
#include "OmManager.h"
#include "gui/OmUiHelpLog.h"

///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
OmUiHelpLog::OmUiHelpLog(HINSTANCE hins) : OmDialog(hins),
  _hFtMonos(Om_createFont(14, 400, L"Consolas"))
{

}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
OmUiHelpLog::~OmUiHelpLog()
{
  if(this->_hFtMonos) DeleteObject(this->_hFtMonos);

  OmManager* pMgr = static_cast<OmManager*>(this->_data);
  pMgr->setLogOutput(nullptr);
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
long OmUiHelpLog::id() const
{
  return IDD_HELP_LOG;
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
void OmUiHelpLog::_onInit()
{
  this->msgItem(IDC_EC_ENT01, WM_SETFONT, reinterpret_cast<WPARAM>(this->_hFtMonos), true);

  OmManager* pMgr = static_cast<OmManager*>(this->_data);
  pMgr->setLogOutput(this->getItem(IDC_EC_ENT01));
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
void OmUiHelpLog::_onResize()
{
  this->_setItemPos(IDC_EC_ENT01, 5, 5, this->width()-10, this->height()-10);
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
bool OmUiHelpLog::_onMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if(uMsg == WM_COMMAND) {

    switch(LOWORD(wParam))
    {
    case IDC_BC_CLOSE:
      this->quit();
      break;
    }

  }

  return false;
}

