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
#ifndef OMUIPROPHUBPST_H
#define OMUIPROPHUBPST_H

#include "OmDialogPropTab.h"

#define HUB_PROP_BAT_ORDER        0
#define HUB_PROP_BAT_QUIETMODE    1

/// \brief Mod Hub Properties / Batches tab child
///
/// OmDialog class derived for Mod Hub Properties / Batches tab child dialog window
///
class OmUiPropHubPst : public OmDialogPropTab
{
  public:

    /// \brief Constructor.
    ///
    /// Default constructor.
    ///
    /// \param[in]  hins    : API Instance handle.
    ///
    OmUiPropHubPst(HINSTANCE hins);

    /// \brief Destructor.
    ///
    /// Default destructor.
    ///
    ~OmUiPropHubPst();

    /// \brief Get resource id.
    ///
    /// Returns dialog window resource id.
    ///
    /// \return dialog resource id.
    ///
    long id() const;

  private: ///          - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void                _lb_pst_on_selchg();

    void                _preset_list_up();

    void                _preset_list_dn();

    void                _preset_dialog_add();

    void                _preset_delete();

    void                _preset_dialog_prop();

    void                _onTbInit();

    void                _onTbResize();

    void                _onTbRefresh();

    INT_PTR             _onTbMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif // OMUIPROPHUBPST_H
