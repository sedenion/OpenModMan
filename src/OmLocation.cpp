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

#include "OmManager.h"
#include "OmContext.h"
#include "OmLocation.h"

/// \brief Package name comparison callback
///
/// std::sort callback comparison function for sorting
/// package by name in alphabetical order.
///
/// \param[in]  a     : Left Package.
/// \param[in]  b     : Right Package.
///
/// \return True if Package a is "before" Package b, false otherwise
///
static bool __pkg_comp_name_fn(const OmPackage* a, const OmPackage* b)
{
  // test against the shorter string
  size_t l = a->name().size() > b->name().size() ? b->name().size() : a->name().size();

  const wchar_t* a_srt = a->name().c_str();
  const wchar_t* b_str = b->name().c_str();

  // test for ASCII value greater than the other
  for(unsigned i = 0; i < l; ++i) {
    if(towupper(a_srt[i]) != towupper(b_str[i])) {
      if(towupper(a_srt[i]) < towupper(b_str[i])) {
        return true;
      } else {
        return false;
      }
    }
  }

  // strings are equals in tester portion, sort by string size
  if(a->name().size() < b->name().size())
    return true;

  // strings are strictly equals, we sort by "IsZip" status
  if(a->isZip() && !b->isZip())
    return true;

  return false;
}

/// \brief Remote package name comparison callback
///
/// std::sort callback comparison function for sorting
/// remote package by name in alphabetical order.
///
/// \param[in]  a     : Left Remote package.
/// \param[in]  b     : Right Remote package.
///
/// \return True if Package a is "before" Package b, false otherwise
///
static bool __rmt_comp_name_fn(const OmRemote* a, const OmRemote* b)
{
  // test against the shorter string
  size_t l = a->name().size() > b->name().size() ? b->name().size() : a->name().size();

  const wchar_t* a_srt = a->name().c_str();
  const wchar_t* b_str = b->name().c_str();

  // test for ASCII value greater than the other
  for(unsigned i = 0; i < l; ++i) {
    if(towupper(a_srt[i]) != towupper(b_str[i])) {
      if(towupper(a_srt[i]) < towupper(b_str[i])) {
        return true;
      } else {
        return false;
      }
    }
  }

  // strings are equals in tester portion, sort by string size
  if(a->name().size() < b->name().size())
    return true;

  return false;
}


/// \brief Package version comparison callback
///
/// std::sort callback comparison function for sorting package
/// by version ascending.
///
/// \param[in]  a     : Left Package.
/// \param[in]  b     : Right Package.
///
/// \return True if Package a is "before" Package b, false otherwise
///
static bool __pkg_comp_vers_fn(const OmPackage* a, const OmPackage* b)
{
  if(a->version() == b->version()) {
    return __pkg_comp_name_fn(a, b);
  } else {
    return (a->version() < b->version());
  }
}


/// \brief Remote package version comparison callback
///
/// std::sort callback comparison function for sorting remote package
/// by version ascending.
///
/// \param[in]  a     : Left Remote package.
/// \param[in]  b     : Right Remote package.
///
/// \return True if Package a is "before" Package b, false otherwise
///
static bool __rmt_comp_vers_fn(const OmRemote* a, const OmRemote* b)
{
  if(a->version() == b->version()) {
    return __rmt_comp_name_fn(a, b);
  } else {
    return (a->version() < b->version());
  }
}


/// \brief Package state comparison callback
///
/// std::sort callback comparison function for sorting package
/// by installation state order.
///
/// \param[in]  a     : Left Package.
/// \param[in]  b     : Right Package.
///
/// \return True if Package a is "before" Package b, false otherwise
///
static bool __pkg_comp_stat_fn(const OmPackage* a, const OmPackage* b)
{
  if(a->hasBck() && b->hasBck()) {
    return __pkg_comp_name_fn(a, b);
  } else {
    return (a->hasBck() && !b->hasBck());
  }
}


/// \brief Remote package state comparison callback
///
/// std::sort callback comparison function for sorting remote package
/// by state order.
///
/// \param[in]  a     : Left Remote package.
/// \param[in]  b     : Right Remote package.
///
/// \return True if Package a is "before" Package b, false otherwise
///
static bool __rmt_comp_stat_fn(const OmRemote* a, const OmRemote* b)
{
  if(a->state() == b->state()) {
    return __rmt_comp_name_fn(a, b);
  } else {
    return (a->state() < b->state());
  }
}


/// \brief Remote package size comparison callback
///
/// std::sort callback comparison function for sorting remote package
/// by size order.
///
/// \param[in]  a     : Left Package.
/// \param[in]  b     : Right Package.
///
/// \return True if Package a is "before" Package b, false otherwise
///
static bool __rmt_comp_size_fn(const OmRemote* a, const OmRemote* b)
{
  if(a->bytes() == b->bytes()) {
    return __rmt_comp_name_fn(a, b);
  } else {
    return (a->bytes() < b->bytes());
  }
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
OmLocation::OmLocation(OmContext* pCtx) :
  _context(pCtx),
  _config(),
  _uuid(),
  _title(),
  _index(0),
  _home(),
  _path(),
  _dstDir(),
  _libDir(),
  _libDirCust(false),
  _bckDir(),
  _bckDirCust(false),
  _pkgLs(),
  _bckZipLevel(-1),
  _pkgSorting(LS_SORT_NAME),
  _rmtSorting(LS_SORT_NAME),
  _valid(false),
  _error()
{

}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
OmLocation::~OmLocation()
{
  this->close();
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
void OmLocation::libSetSorting(OmLocLsSort sorting)
{
  // we check if the requested sorting kind is the same as the currently
  // used, in this case, this mean the sorting order must be reversed

  if(sorting & this->_pkgSorting) {

    // check if current sorting is reversed, then switch order
    if(this->_pkgSorting & LS_SORT_REVERSE) {
      this->_pkgSorting &= ~LS_SORT_REVERSE; //< remove reversed flag
    } else {
      this->_pkgSorting |= LS_SORT_REVERSE; //< add reversed flag
    }

  } else {

    this->_pkgSorting = sorting;
  }

  // save the current sorting
  if(this->_config.valid()) {

    if(this->_config.xml().hasChild(L"library_sort")) {
      this->_config.xml().child(L"library_sort").setAttr(L"sort", static_cast<int>(this->_pkgSorting));
    } else {
      this->_config.xml().addChild(L"library_sort").setAttr(L"sort", static_cast<int>(this->_pkgSorting));
    }

    this->_config.save();
  }

  // finally sort packages
  this->_pkgSort();
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
bool OmLocation::open(const wstring& path)
{
  wstring verbose; //< for log message compositing

  this->close();

  this->log(2, L"Location(<anonymous>) Load", L"\""+path+L"\"");

  // try to open and parse the XML file
  if(!this->_config.open(path, OMM_CFG_SIGN_LOC)) {
    this->_error =  L"\"" + Om_getFilePart(path);
    this->_error += L"\" definition file open error: ";
    this->_error += this->_config.lastErrorStr();
    this->log(0, L"Location(<anonymous>) Load", this->_error);
    return false;
  }

  // check for the presence of <uuid> entry
  if(!this->_config.xml().hasChild(L"uuid")) {
    this->_error =  L"\"" + Om_getFilePart(path);
    this->_error += L"\" invalid definition: <uuid> node missing.";
    log(0, L"Location(<anonymous>) Load", this->_error);
    return false;
  }

  // check for the presence of <title> entry
  if(!this->_config.xml().hasChild(L"title")) {
    this->_error =  L"\"" + Om_getFilePart(path);
    this->_error += L"\" invalid definition: <title> node missing.";
    log(0, L"Location(<anonymous>) Load", this->_error);
    return false;
  }

  // at this point the Location may be valid
  this->_path = path;
  this->_home = Om_getDirPart(this->_path);
  this->_uuid = this->_config.xml().child(L"uuid").content();
  this->_title = this->_config.xml().child(L"title").content();
  this->_index = this->_config.xml().child(L"title").attrAsInt(L"index");

  // check for the presence of <install> entry
  if(this->_config.xml().hasChild(L"install")) {
    // we check whether destination folder is valid
    this->_dstDir = this->_config.xml().child(L"install").content();
    if(!Om_isDir(this->_dstDir)) {
      verbose = L"Destination folder \""+this->_dstDir+L"\"";
      verbose += OMM_STR_ERR_ISDIR;
      this->log(1, L"Location("+this->_title+L") Load", verbose);
    } else {
      verbose = L"Using destination folder: \"";
      verbose += this->_dstDir + L"\".";
      this->log(2, L"Location("+this->_title+L") Load", verbose);
    }
  } else {
    this->_error = L"Invalid definition: <install> node missing.";
    this->log(0, L"Location("+this->_title+L") Load", this->_error);
    this->close();
    return false;
  }

  // check for the presence of <library> entry for custom Library path
  if(this->_config.xml().hasChild(L"library")) {
    // get the custom Library path in config
    this->_libDir = this->_config.xml().child(L"library").content();
    // notify we use a custom Library path
    this->_libDirCust = true;
    if(!Om_isDir(this->_libDir)) {
      verbose = L"Custom library folder \""+this->_libDir+L"\"";
      verbose += OMM_STR_ERR_ISDIR;
      this->log(1, L"Location("+this->_title+L") Load", verbose);
    } else {
      verbose = L"Using custom library folder: \""+this->_libDir+L"\".";
      this->log(2, L"Location("+this->_title+L") Load", verbose);
    }
  } else {
    // no <library> node in config, use default settings
    this->_libDir = this->_home + L"\\library";
    if(!Om_isDir(this->_libDir)) {
      int result = Om_dirCreate(this->_libDir);
      if(result != 0) {
        this->_error = L"Default library folder \""+this->_libDir+L"\".";
        this->_error += OMM_STR_ERR_CREATE(Om_getErrorStr(result));
        this->log(0, L"Location("+this->_title+L") Load", this->_error);
        this->close();
        return false;
      }
    }
    verbose = L"Using default library folder: \""+this->_libDir+L"\".";
    this->log(2, L"Location("+this->_title+L") Load", verbose);
  }

  // check for the presence of <backup> entry for custom Backup path
  if(this->_config.xml().hasChild(L"backup")) {
    // get the custom Backup path in config
    this->_bckDir = this->_config.xml().child(L"backup").content();
    // notify we use a custom Backup path
    this->_bckDirCust = true;
    if(!Om_isDir(this->_bckDir)) {
      verbose = L"Custom backup folder \""+this->_bckDir+L"\"";
      verbose += OMM_STR_ERR_ISDIR;
      this->log(1, L"Location("+this->_title+L") Load", verbose);
    } else {
      verbose = L"Using custom backup folder: \""+this->_bckDir+L"\".";
      this->log(2, L"Location("+this->_title+L") Load", verbose);
    }
  } else {
    // no <backup> node in config, use default settings
    this->_bckDir = this->_home + L"\\backup";
    if(!Om_isDir(this->_bckDir)) {
      int result = Om_dirCreate(this->_bckDir);
      if(result != 0) {
        this->_error = L"Default backup folder \""+this->_bckDir+L"\"";
        this->_error += OMM_STR_ERR_CREATE(Om_getErrorStr(result));
        this->log(0, L"Location("+this->_title+L") Load", this->_error);
        this->close();
        return false;
      }
    }
    verbose = L"Using default backup folder: \""+this->_bckDir+L"\".";
    this->log(2, L"Location("+this->_title+L") Load", verbose);
  }

  // we check for backup compression level
  if(this->_config.xml().hasChild(L"backup_comp")) {
    this->_bckZipLevel = this->_config.xml().child(L"backup_comp").attrAsInt(L"level");

    // check whether we have a correct value
    if(this->_bckZipLevel > 3)
      this->_bckZipLevel = -1;
  }

  // we check for saved library sorting
  if(this->_config.xml().hasChild(L"library_sort")) {
    this->_pkgSorting = this->_config.xml().child(L"library_sort").attrAsInt(L"sort");
  }

  // we check for saved remotes sorting
  if(this->_config.xml().hasChild(L"remotes_sort")) {
    this->_pkgSorting = this->_config.xml().child(L"remotes_sort").attrAsInt(L"sort");
  }

  // Get network repository list
  if(this->_config.xml().hasChild(L"network")) {

    OmXmlNode xml_net = this->_config.xml().child(L"network");

    if(xml_net.hasAttr(L"upgd_rename")) {
      this->_upgdRename = xml_net.attrAsInt(L"upgd_rename");
    } else {
      this->_upgdRename = false;
    }

    // check whether repository already exists
    vector<OmXmlNode> xml_rep_list;
    xml_net.children(xml_rep_list, L"repository");

    OmRepository* pRep;

    for(size_t i = 0; i < xml_rep_list.size(); ++i) {
      pRep = new OmRepository(this);
      if(pRep->init(xml_rep_list[i].attrAsString(L"base"), xml_rep_list[i].attrAsString(L"name"))) {
        this->_repLs.push_back(pRep);
        verbose = L"Add Repository: \""+pRep->url()+L"\".";
        this->log(2, L"Location("+this->_title+L") Load", verbose);
      } else {
        delete pRep;
      }
    }
  }

  // this location is OK and ready
  this->_valid = true;

  this->log(2, L"Location("+this->_title+L") Load", L"Success");

  // Refresh library
  this->libRefresh();

  return true;
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
void OmLocation::close()
{
  wstring title = this->_title;

  this->libClear();
  this->_home.clear();
  this->_path.clear();
  this->_title.clear();
  this->_dstDir.clear();
  this->_libDir.clear();
  this->_libDirCust = false;
  this->_bckDir.clear();
  this->_bckDirCust = false;
  this->_config.close();
  this->_valid = false;

  this->log(2, L"Location("+title+L") Close", L"Success");
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
bool OmLocation::checkAccessLib()
{
  bool access_ok = true;

  // checks whether folder exists
  if(Om_isDir(this->_libDir)) {
    // checks for proper permissions on folder
    if(Om_checkAccess(this->_libDir, OMM_ACCESS_DIR_READ)) {
      if(!Om_checkAccess(this->_libDir, OMM_ACCESS_DIR_WRITE)) {
        this->_error = L"Library folder \""+this->_libDir+L"\"";
        this->_error += OMM_STR_ERR_WRITE;
        access_ok = false;
      }
    } else {
      this->_error =  L"Library folder \""+this->_libDir+L"\"";
      this->_error += OMM_STR_ERR_READ;
      access_ok = false;
    }
  } else {
    if(this->_libDirCust) {
      this->_error =  L"Custom library folder \""+this->_libDir+L"\"";
      this->_error += OMM_STR_ERR_ISDIR;
      access_ok = false;
    } else {
      // try to create it
      int result = Om_dirCreate(this->_libDir);
      if(result != 0) {
        this->_error = L"Default library folder \""+this->_libDir+L"\"";
        this->_error += OMM_STR_ERR_CREATE(Om_getErrorStr(result));
        access_ok = false;
      }
    }
  }

  if(!access_ok) {

    this->log(0, L"Location("+this->_title+L") Library access", this->_error);

    return false;
  }

  return true;
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
bool OmLocation::checkAccessBck()
{
  bool access_ok = true;

  // checks whether folder exists
  if(Om_isDir(this->_bckDir)) {
    // checks for proper permissions on folder
    if(Om_checkAccess(this->_bckDir, OMM_ACCESS_DIR_READ)) {
      if(!Om_checkAccess(this->_bckDir, OMM_ACCESS_DIR_WRITE)) {
        this->_error = L"Backup folder \""+this->_bckDir+L"\"";
        this->_error += OMM_STR_ERR_WRITE;
        access_ok = false;
      }
    } else {
      this->_error = L"Backup folder \""+this->_bckDir+L"\"";
      this->_error += OMM_STR_ERR_READ;
      access_ok = false;
    }
  } else {
    if(this->_bckDirCust) {
      this->_error =  L"Custom backup folder \""+this->_bckDir+L"\"";
      this->_error += OMM_STR_ERR_ISDIR;
      access_ok = false;
    } else {
      // try to create it
      int result = Om_dirCreate(this->_bckDir);
      if(result != 0) {
        this->_error = L"Default backup folder \""+this->_bckDir+L"\"";
        this->_error += OMM_STR_ERR_CREATE(Om_getErrorStr(result));
        access_ok = false;
      }
    }
  }

  if(!access_ok) {

    this->log(0, L"Location("+this->_title+L") Backup access", this->_error);

    return false;
  }

  return true;
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
bool OmLocation::checkAccessDst()
{
  bool access_ok = true;

  // checks whether folder exists
  if(Om_isDir(this->_dstDir)) {
    // checks for proper permissions on folder
    if(Om_checkAccess(this->_dstDir, OMM_ACCESS_DIR_READ)) {
      if(!Om_checkAccess(this->_dstDir, OMM_ACCESS_DIR_WRITE)) {
        this->_error = L"Destination folder \""+this->_dstDir+L"\"";
        this->_error += OMM_STR_ERR_WRITE;
        access_ok = false;
      }
    } else {
      this->_error = L"Destination folder \""+this->_dstDir+L"\"";
      this->_error += OMM_STR_ERR_READ;
      access_ok = false;
    }
  } else {
    this->_error =  L"Destination folder \""+this->_dstDir+L"\"";
    this->_error += OMM_STR_ERR_ISDIR;
    access_ok = false;
  }

  if(!access_ok) {

    this->log(0, L"Location("+this->_title+L") Destination access", this->_error);

    return false;
  }

  return true;
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
void OmLocation::libClear()
{
  for(size_t i = 0; i < this->_pkgLs.size(); ++i)
    delete this->_pkgLs[i];
  this->_pkgLs.clear();
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
bool OmLocation::libRefresh()
{
  // some explanation about how Packages and Backups are managed...
  //
  // One Package object can incarnate two totally different things which
  // seam to be one unique thing in the user point of view:
  // A "Backup" and a "Source".
  //
  // The "Source" refers to the Package itself, i.e, the Mod's files to be
  // installed into the Destination folder.
  // The "Backup" refer to original application files saved in a safe place
  // before they were overwritten by the Mod's modified files.
  //
  // So, the Package object is double-sided, one side is the "Backup", the
  // other is the "Source". It may "be" either one, another, or both at the
  // same time, depending which element is actually available.
  //
  //   Backup File/Dir
  //            \ ______ Package
  //            /        Object
  //   Source File/Dir
  //
  // In this function, we create the Packages list the user will manipulate and
  // Package objects will be created with one, or both side depending what is
  // found in backup and library folders.
  //
  // "Source" and "Backup" are linked together using a unsigned 64 bit integer
  // xxHash value created from the "Source" Package file (or folder) name.
  //
  // When a "Source" is installed, this Hash is stored in a Backup definition
  // file within the Backup zip file or sub-folder, allowing to matches the
  // available Backups with available Sources.
  //
  // Notice that if user renames its package "Source" file, it will no longer
  // be linkable to an existing "Backup", it will be threated as a separate
  // Package.


  // This function is called either to initializes the package list, or to
  // refresh it because of a change in the Library folder.
  //
  // We want to avoid re-parsing all Backup and Sources for a small changes in
  // the Library folder or Backup status, so, if the package list if not empty,
  // we only update only the necessary.
  //
  // Notice that Backup are tracked internally, the algorithm does not support
  // changes in Backup folder by third-party.

  vector<wstring> path_ls;
  OmPackage* pPkg;

  // track list changes
  bool changed = false;

  // our package list is not empty, we will check for added or removed item
  if(this->_pkgLs.size()) {

    // get content of the package Library folder
    Om_lsFileFiltered(&path_ls, this->_libDir, L"*.zip", true);
    Om_lsFileFiltered(&path_ls, this->_libDir, L"*.omp", true);
    if(this->_context->_manager->legacySupport())
      Om_lsDir(&path_ls, this->_libDir, true);

    bool in_list;

    // search for unavailable Sources
    for(size_t p = 0; p < this->_pkgLs.size(); ++p) {

      // search this Source in Library folder item list
      in_list = false;
      for(size_t k = 0; k < path_ls.size(); ++k) {
        if(this->_pkgLs[p]->srcPath() == path_ls[k]) { //< compare Source paths
          in_list = true; break;
        }
      }

      // this Source is no longer in Library folder
      if(!in_list) {
        changed = true;
        // check whether this Package is installed (has backup)
        if(this->_pkgLs[p]->hasBck()) {
          // this Package is installed, in this case we don't remove it from
          // list, but we revoke its "Source" property since it is no longer
          // available
          this->_pkgLs[p]->srcClear();
        } else {
          // The Package has no Backup and Source is no longer available
          // this is a ghost, we have to remove it
          this->_pkgLs.erase(this->_pkgLs.begin()+p);
          --p;
        }
      }
    }

    uint64_t pkg_hash;

    // Search for new Sources
    for(size_t i = 0; i < path_ls.size(); ++i) {
      // search in all packages to found this file
      in_list = false;

      pkg_hash = Om_getXXHash3(Om_getFilePart(path_ls[i]));

      for(size_t p = 0; p < this->_pkgLs.size(); ++p) {
        // we first test against the Source Path
        if(path_ls[i] == this->_pkgLs[p]->srcPath()) {
          in_list = true; break;
        }
        // checks whether Hash values matches
        if(pkg_hash == this->_pkgLs[p]->hash()) {
          // this Package Source obviously matches to a currently
          // installed one, since we got a Package with the same Hash but
          // Source is missing, so we add the Source to this Package Backup
          changed = true;
          this->_pkgLs[p]->srcParse(path_ls[i]);
          in_list = true; break;
        }
      }
      // This is a new Package Source
      if(!in_list) {
        pPkg = new OmPackage(this);
        if(pPkg->srcParse(path_ls[i])) {
          changed = true;
          this->_pkgLs.push_back(pPkg);
        } else {
          delete pPkg;
        }
      }
    }

  } else {

    changed = true;

    // get Backup folder content
    Om_lsFileFiltered(&path_ls, this->_bckDir, L"*.zip", true);
    Om_lsFileFiltered(&path_ls, this->_bckDir, L"*.omk", true);
    Om_lsDir(&path_ls, this->_bckDir, true);

    // add all available and valid Backups
    for(size_t i = 0; i < path_ls.size(); ++i) {
      pPkg = new OmPackage(this);
      if(pPkg->bckParse(path_ls[i])) {
        this->_pkgLs.push_back(pPkg);
      } else {
        delete pPkg;
      }
    }

    // get Library folder content
    path_ls.clear();
    Om_lsFileFiltered(&path_ls, this->_libDir, L"*.zip", true);
    Om_lsFileFiltered(&path_ls, this->_libDir, L"*.omp", true);
    if(this->_context->_manager->legacySupport())
      Om_lsDir(&path_ls, this->_libDir, true);

    bool has_bck;

    // Link Sources to matching Backup, or add new Sources
    for(size_t i = 0; i < path_ls.size(); ++i) {
      has_bck = false;
      // check whether this Source matches an existing Backup
      for(size_t p = 0; p < this->_pkgLs.size(); p++) {
        if(this->_pkgLs[p]->isBckOf(path_ls[i])) {
          this->_pkgLs[p]->srcParse(path_ls[i]);
          has_bck = true;
          break;
        }
      }
      // non Backup found for this Source, adding new Source
      if(!has_bck) {
        pPkg = new OmPackage(this);
        if(pPkg->srcParse(path_ls[i])) {
          this->_pkgLs.push_back(pPkg);
        } else {
          delete pPkg;
        }
      }
    }
  }

  if(changed)
    this->_pkgSort();

  #ifdef DEBUG
  std::cout << "DEBUG => OmLocation::libRefresh " << (changed ? "+-" : "==") << "\n";
  #endif

  return changed;
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
bool OmLocation::libClean()
{
  bool changed = false;

  // search for ghost packages
  for(size_t p = 0; p < this->_pkgLs.size(); ++p) {

    if(!this->_pkgLs[p]->hasBck() && !this->_pkgLs[p]->hasSrc()) {
      // The Package has no Backup and Source is no longer available
      // this is a ghost, we have to remove it
      changed = true;
      this->_pkgLs.erase(this->_pkgLs.begin()+p);
      --p;
    }
  }

  #ifdef DEBUG
  std::cout << "DEBUG => OmLocation::libClean " << (changed ? "+-" : "==") << "\n";
  #endif

  return changed;
}

///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
void OmLocation::setTitle(const wstring& title)
{
  if(this->_config.valid()) {

    this->_title = title;

    if(this->_config.xml().hasChild(L"title")) {
      this->_config.xml().child(L"title").setContent(title);
    } else {
      this->_config.xml().addChild(L"title").setContent(title);
    }

    this->_config.save();
  }
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
void OmLocation::setIndex(unsigned index)
{
  if(this->_config.valid()) {

    this->_index = index;

    if(this->_config.xml().hasChild(L"title")) {
      this->_config.xml().child(L"title").setAttr(L"index", static_cast<int>(index));
    }

    this->_config.save();
  }
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
void OmLocation::setDstDir(const wstring& path)
{
  if(this->_config.valid()) {

    this->_dstDir = path;

    if(this->_config.xml().hasChild(L"install")) {
      this->_config.xml().child(L"install").setContent(path);
    } else {
      this->_config.xml().addChild(L"install").setContent(path);
    }

    this->_config.save();
  }
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
void OmLocation::setCustLibDir(const wstring& path)
{
  if(this->_config.valid()) {

    this->_libDir = path;
    // notify we use a custom Library path
    this->_libDirCust = true;

    if(this->_config.xml().hasChild(L"library")) {
      this->_config.xml().child(L"library").setContent(path);
    } else {
      this->_config.xml().addChild(L"library").setContent(path);
    }

    this->_config.save();
  }

  // Force a full refresh for the next time
  this->libClear();
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
void OmLocation::remCustLibDir()
{
  if(this->_config.valid()) {

    this->_libDir = this->_home + L"\\Library";
    // notify we use default settings
    this->_libDirCust = false;

    if(this->_config.xml().hasChild(L"library")) {
      this->_config.xml().remChild(L"library");
    }

    this->_config.save();
  }

  // Force a full refresh for the next time
  this->libClear();
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
void OmLocation::setCustBckDir(const wstring& path)
{
  if(this->_config.valid()) {

    this->_bckDir = path;
    // notify we use a custom Library path
    this->_bckDirCust = true;

    if(this->_config.xml().hasChild(L"backup")) {
      this->_config.xml().child(L"backup").setContent(path);
    } else {
      this->_config.xml().addChild(L"backup").setContent(path);
    }

    this->_config.save();
  }

  // Force a full refresh for the next time
  this->libClear();
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
void OmLocation::remCustBckDir()
{
  if(this->_config.valid()) {

    this->_bckDir = this->_home + L"\\Backup";
    // notify we use default settings
    this->_bckDirCust = false;

    if(this->_config.xml().hasChild(L"backup")) {
      this->_config.xml().remChild(L"backup");
    }

    this->_config.save();
  }

  // Force a full refresh for the next time
  this->libClear();
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
void OmLocation::setBckZipLevel(int level)
{
  if(this->_config.valid()) {

    this->_bckZipLevel = level;

    if(this->_config.xml().hasChild(L"backup_comp")) {
      this->_config.xml().child(L"backup_comp").setAttr(L"level", (int)level);
    } else {
      this->_config.xml().addChild(L"backup_comp").setAttr(L"level", (int)level);
    }

    this->_config.save();
  }
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
void OmLocation::setUpgdRename(bool enable)
{
  if(this->_config.valid()) {

    this->_upgdRename = enable;

    OmXmlNode xml_net;

    if(!this->_config.xml().hasChild(L"network")) {
      xml_net = this->_config.xml().addChild(L"network");
    } else {
      xml_net = this->_config.xml().child(L"network");
    }

    xml_net.setAttr(L"upgd_rename", static_cast<int>(enable ? 1 : 0));

    this->_config.save();
  }
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
bool OmLocation::renameHome(const wstring& name)
{
  wstring title = this->_title;
  wstring old_path = this->_path;
  wstring old_home = this->_home;

  // Close Location to safe rename and reload it after
  this->close();

  bool has_error = false;

  // compose new Location definition file name
  wstring new_file = name;
  new_file += L".";
  new_file += OMM_LOC_FILE_EXT;

  // Rename Location definition file
  int result = Om_fileMove(old_path, old_home + L"\\" + new_file);
  if(result != 0) {
    this->_error =  L"Definition file \""+old_path+L"\"";
    this->_error += OMM_STR_ERR_RENAME(Om_getErrorStr(result));
    this->log(0, L"Location("+title+L") Rename", this->_error);
    // get back the old file name to restore Location properly
    new_file = Om_getFilePart(old_path);
    has_error = true;
  } else {
    this->log(2, L"Location("+title+L") Rename", L"definition file renamed to \""+new_file+L"\"");
  }

  // compose new Location home folder
  wstring new_home = old_home.substr(0, old_home.find_last_of(L"\\") + 1);
  new_home += name;

  std::wcout << new_home << L"\n";

  // Rename Location home folder
  result = Om_fileMove(old_home, new_home);
  if(result != 0) {
    this->_error =  L"Location subfolder \""+old_home+L"\"";
    this->_error += OMM_STR_ERR_RENAME(Om_getErrorStr(result));
    this->log(0, L"Location("+title+L") Rename", this->_error);
    // get back the old home folder to restore Location properly
    new_home = old_home;
    has_error = true;
  } else {
    this->log(2, L"Location("+title+L") Rename", L"subfolder renamed to \""+new_home+L"\"");
  }

  // Reload location
  this->open(new_home + L"\\" + new_file);

  if(has_error) {
    this->log(2, L"Location("+title+L") Rename", L"Failed");
  } else {
    this->log(2, L"Location("+title+L") Rename", L"Success");
  }

  return !has_error;
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
bool OmLocation::bckHasData()
{
  for(size_t i = 0; i < _pkgLs.size(); ++i) {
    if(_pkgLs[i]->hasBck()) return true;
  }
  return false;
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
bool OmLocation::bckPurge(Om_progressCb progress_cb, void* user_ptr)
{
  // checks for access to backup folder
  if(!this->checkAccessBck()) {
    this->_error =  L"Backup folder \""+this->_bckDir+L"\"";
    this->_error += OMM_STR_ERR_DIRACCESS;
    this->log(1, L"Location("+this->_title+L") Purge backups", this->_error);
    return false;
  }
  // checks for access to destination folder
  if(!this->checkAccessDst()) {
    this->_error =  L"Destination folder \""+this->_dstDir+L"\"";
    this->_error += OMM_STR_ERR_DIRACCESS;
    this->log(1, L"Location("+this->_title+L") Purge backups", this->_error);
    return false;
  }

  // get list of all installed packages
  vector<OmPackage*> pkg_ls; //< our select list
  for(size_t i = 0; i < this->_pkgLs.size(); ++i)
    if(this->_pkgLs[i]->hasBck())
      pkg_ls.push_back(this->_pkgLs[i]);

  // if no package installed, nothing to purge
  if(pkg_ls.empty())
    return true;

  // initialize progression
  size_t progress_tot, progress_cur;
  if(progress_cb) {
    progress_tot = pkg_ls.size();
    progress_cur = 0;
    if(!progress_cb(user_ptr, progress_tot, progress_cur, nullptr))
      return true;
  }

  // even if we uninstall all packages, we need to get a sorted list
  // so we prepare with all overlaps and dependencies checking
  vector<OmPackage*> over_ls;
  vector<OmPackage*> deps_ls;
  vector<OmPackage*> unin_ls;

  // prepare packages uninstall and backups restoration
  this->bckPrepareUnin(unin_ls, over_ls, deps_ls, pkg_ls);

  bool has_error = false;

  this->log(2, L"Location("+this->_title+L") Purge backups", L"Uninstalling "+to_wstring(unin_ls.size())+L" packages.");

  unsigned n = 0;

  // here we go for uninstall all packages
  for(size_t i = 0; i < unin_ls.size(); ++i) {

    // call progression callback
    if(progress_cb) {
      progress_cur++;
      if(!progress_cb(user_ptr, progress_tot, progress_cur, unin_ls[i]->ident().c_str()))
        break;
    }

    // uninstall package
    if(unin_ls[i]->uninst()) {
      n++; //< increase counter
    } else {
      this->_error =  L"Package \""+unin_ls[i]->ident()+L"\"";
      this->_error += L" uninstall failed: "+unin_ls[i]->lastError();
      this->log(1, L"Location("+this->_title+L") Purge backups", this->_error);
      has_error = true;
    }

    #ifdef DEBUG
    Sleep(OMM_DEBUG_SLOW); //< for debug
    #endif
  }

  // refresh library
  this->libRefresh();

  this->log(2, L"Location("+this->_title+L") Purge backups", to_wstring(n)+L" backups successfully restored.");

  return !has_error;
}

///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
bool OmLocation::bckMove(const wstring& path, Om_progressCb progress_cb, void* user_ptr)
{
  if(path == this->_bckDir)
    return true;

  // verify backup folder access
  if(!this->checkAccessBck()) {
    this->_error =  L"Backup folder \""+this->_bckDir+L"\"";
    this->_error += OMM_STR_ERR_DIRACCESS;
    this->log(1, L"Location("+this->_title+L") Move backups", this->_error);
    return false;
  }

  // get content of backup folder
  vector<wstring> path_ls;
  Om_lsAll(&path_ls, this->_bckDir, false);

  // initialize progression
  size_t progress_tot, progress_cur;
  if(progress_cb) {
    progress_tot = path_ls.size();
    progress_cur = 0;
    if(!progress_cb(user_ptr, progress_tot, progress_cur, nullptr))
      return true;
  }

  bool has_error = false;

  this->log(2, L"Location("+this->_title+L") Purge backups", L"Moving "+to_wstring(path_ls.size())+L" elements.");

  unsigned n = 0;
  int result;
  wstring src, dst;

  for(size_t i = 0; i < path_ls.size(); ++i) {

    // call progression callback
    if(progress_cb) {
      progress_cur++;
      if(!progress_cb(user_ptr, progress_tot, progress_cur, path_ls[i].c_str()))
        break;
    }

    // compose source and destination path
    src = this->_bckDir + L"\\" + path_ls[i];
    dst = path + L"\\" + path_ls[i];

    // move file
    result = Om_fileMove(src, dst);
    if(result != 0) {
      this->_error =  L"Backup data \""+src+L"\"";
      this->_error += OMM_STR_ERR_MOVE(Om_getErrorStr(result));
      this->log(1, L"Location("+this->_title+L") Move backups", this->_error);
      has_error = true;
    } else {
      n++; //< increase counter
    }

    #ifdef DEBUG
    Sleep(OMM_DEBUG_SLOW); //< for debug
    #endif
  }

  // full refresh library
  this->libClear();
  this->libRefresh();

  this->log(2, L"Location("+this->_title+L") Move backups", to_wstring(n)+L" elements successfully transfered.");

  return !has_error;
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
bool OmLocation::bckDcard(Om_progressCb progress_cb, void* user_ptr)
{
  // verify backup folder access
  if(!this->checkAccessBck()) {
    this->_error =  L"Backup folder \""+this->_bckDir+L"\"";
    this->_error += OMM_STR_ERR_DIRACCESS;
    this->log(1, L"Location("+this->_title+L") Move backups", this->_error);
    return false;
  }

  // gather all installed packages
  vector<OmPackage*> pkg_ls;
  for(size_t i = 0; i < this->_pkgLs.size(); ++i) {
    if(this->_pkgLs[i]->hasBck())
      pkg_ls.push_back(this->_pkgLs[i]);
  }

  // check whether we have something to proceed
  if(pkg_ls.empty())
    return true;

  // initialize progression
  size_t progress_tot, progress_cur;
  if(progress_cb) {
    progress_tot = pkg_ls.size();
    progress_cur = 0;
    if(!progress_cb(user_ptr, progress_tot, progress_cur, nullptr))
      return true;
  }

  bool has_error = false;

  this->log(2, L"Location("+this->_title+L") Purge backups", L"Discarding "+to_wstring(pkg_ls.size())+L" backups.");

  unsigned n = 0;

  // Discard backup data for all packages
  for(size_t i = 0; i < pkg_ls.size(); ++i) {

    // call progression callback
    if(progress_cb) {
      progress_cur++;
      if(!progress_cb(user_ptr, progress_tot, progress_cur, pkg_ls[i]->ident().c_str()))
        break;
    }

    // Discard backup of this package
    if(pkg_ls[i]->unbackup()) {
      n++; //< increase counter
    } else {
      this->_error =  L"Backup data \""+pkg_ls[i]->name()+L"\"";
      this->_error += L" discard failed: "+pkg_ls[i]->lastError();
      this->log(0, L"Location("+this->_title+L") Discard backups", this->_error);
      has_error = true;
    }

    #ifdef DEBUG
    Sleep(OMM_DEBUG_SLOW); //< for debug
    #endif
  }

  // refresh library
  this->libRefresh();

  this->log(2, L"Location("+this->_title+L") Move backups", to_wstring(n)+L" backup successfully discarded.");

  return !has_error;
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
void OmLocation::pkgPrepareInst(vector<OmPackage*>& ins_ls, vector<OmPackage*>& ovr_ls, vector<OmPackage*>& dep_ls, vector<wstring>& mis_ls, const vector<OmPackage*>& pkg_ls) const
{
  // gather dependencies and create missing lists
  vector<wstring> idt_ls;
  for(size_t i = 0; i < pkg_ls.size(); ++i) {

    idt_ls.clear();
    this->pkgGetDepends(ins_ls, idt_ls, pkg_ls[i]);

    for(size_t j = 0; j < idt_ls.size(); ++j) {
      // add uniques only
      if(std::find(mis_ls.begin(), mis_ls.end(), idt_ls[j]) == mis_ls.end()) {
        mis_ls.push_back(idt_ls[j]);
      }
    }
  }

  // create the extra install list
  for(size_t i = 0; i < ins_ls.size(); ++i) {
    // add only if not in the initial selection
    if(std::find(pkg_ls.begin(), pkg_ls.end(), ins_ls[i]) == pkg_ls.end()) {
      dep_ls.push_back(ins_ls[i]);
    }
  }

  // compose the final install list
  for(size_t i = 0; i < pkg_ls.size(); ++i) {
    // add only if not already in install list
    if(std::find(ins_ls.begin(), ins_ls.end(), pkg_ls[i]) == ins_ls.end()) {
      ins_ls.push_back(pkg_ls[i]);
    }
  }

  // get installation footprint of packages to be installed
  vector<OmPkgItem> footp;
  vector<vector<OmPkgItem>> footp_ls;

  // get overlaps list including simulated installation
  for(size_t i = 0; i < ins_ls.size(); ++i) {

    // test overlapping against installed packages
    for(size_t j = 0; j < this->_pkgLs.size(); ++j) {
      if(this->_pkgLs[j]->hasBck()) {
        if(ins_ls[i]->ovrTest(this->_pkgLs[j]))
          ovr_ls.push_back(this->_pkgLs[j]);
      }
    }

    // test overlapping against packages to be installed
    for(size_t j = 0; j < footp_ls.size(); ++j) {
      if(ins_ls[i]->ovrTest(footp_ls[j]))
        ovr_ls.push_back(ins_ls[j]);
    }

    // create installation footprint of package
    footp.clear();
    ins_ls[i]->footprint(footp);
    footp_ls.push_back(footp);
  }
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
void OmLocation::bckPrepareUnin(vector<OmPackage*>& uns_ls, vector<OmPackage*>& ovr_ls,  vector<OmPackage*>& dep_ls, const vector<OmPackage*>& pkg_ls) const
{
  // get overlapping packages list to be uninstalled before selection
  for(size_t i = 0; i < pkg_ls.size(); ++i) {

    // this is the only call we do, but the function is doubly recursive and
    // can lead to huge complexity depending the actual state of package installation
    // dependencies and overlapping...
    this->bckGetExtras(uns_ls, ovr_ls, dep_ls, pkg_ls[i]);
  }

  // compose the final uninstall list
  for(size_t i = 0; i < pkg_ls.size(); ++i) {
    // add only if not already in initial list
    if(std::find(uns_ls.begin(), uns_ls.end(), pkg_ls[i]) == uns_ls.end()) {
      uns_ls.push_back(pkg_ls[i]);
    }
  }
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
size_t OmLocation::pkgFindOverlaps(vector<OmPackage*>& pkg_list, const OmPackage* pkg) const
{
  size_t n = 0;

  for(size_t i = 0; i < _pkgLs.size(); ++i) {
    if(this->_pkgLs[i]->hasBck()) {
      if(pkg->ovrTest(this->_pkgLs[i])) {
        pkg_list.push_back(this->_pkgLs[i]);
        ++n;
      }
    }
  }

  return n;
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
size_t OmLocation::pkgFindOverlaps(vector<uint64_t>& hash_list, const OmPackage* pkg) const
{
  size_t n = 0;

  for(size_t i = 0; i < _pkgLs.size(); ++i) {
    if(this->_pkgLs[i]->hasBck()) {
      if(pkg->ovrTest(this->_pkgLs[i])) {
        hash_list.push_back(this->_pkgLs[i]->hash());
        ++n;
      }
    }
  }

  return n;
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
size_t OmLocation::pkgGetDepends(vector<OmPackage*>& dep_ls, vector<wstring>& mis_ls, const OmPackage* pkg) const
{
  size_t n = 0;

  bool missing;

  for(size_t i = 0; i < pkg->depCount(); ++i) {

    missing = true;

    for(size_t j = 0; j < this->_pkgLs.size(); ++j) {

      // rely only on packages
      if(!this->_pkgLs[j]->isZip())
        continue;

      if(pkg->depGet(i) == this->_pkgLs[j]->ident()) {

        n += this->pkgGetDepends(dep_ls, mis_ls, this->_pkgLs[j]);
        // we add to list only if unique and not already installed, this allow
        // us to get a consistent dependency list for a bunch of package by
        // calling this function for each package without clearing the list
        if(!this->_pkgLs[j]->hasBck()) {
          if(std::find(dep_ls.begin(), dep_ls.end(), this->_pkgLs[j]) == dep_ls.end()) {
            dep_ls.push_back(this->_pkgLs[j]);
            ++n;
          }
        }

        missing = false;
        break;
      }
    }

    if(missing) {
      // we add to list only if unique
      if(std::find(mis_ls.begin(), mis_ls.end(), pkg->depGet(i)) == mis_ls.end()) {
        mis_ls.push_back(pkg->depGet(i));
      }
    }
  }

  return n;
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
size_t OmLocation::bckGetOverlaps(vector<OmPackage*>& pkg_list, const OmPackage* pkg) const
{
  size_t n = 0;
  for(size_t i = 0; i < this->_pkgLs.size(); ++i) {

    // search only among installed packages
    if(!this->_pkgLs[i]->hasBck())
      continue;

    if(this->_pkgLs[i]->ovrHas(pkg->_hash)) {
      // the function is recursive, we want the full list like a
      // depth-first search in the right order
      n += this->bckGetOverlaps(pkg_list, this->_pkgLs[i]);
      // recursive way can produce doubles, we want to avoid it
      // so we add only if not already in the list
      if(std::find(pkg_list.begin(), pkg_list.end(), this->_pkgLs[i]) == pkg_list.end()) {
        pkg_list.push_back(this->_pkgLs[i]);
        ++n;
      }
    }
  }
  return n;
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
size_t OmLocation::bckGetNeedies(vector<OmPackage*>& pkg_list, const OmPackage* pkg) const
{
  size_t n = 0;
  for(size_t i = 0; i < this->_pkgLs.size(); ++i) {

    // search only among installed packages
    if(!this->_pkgLs[i]->hasBck())
      continue;

    if(this->_pkgLs[i]->depHas(pkg->_ident)) {
      // check recursively, this give depth-first sorted list
      n += this->bckGetNeedies(pkg_list, this->_pkgLs[i]);
      // add only if unique
      if(std::find(pkg_list.begin(), pkg_list.end(), this->_pkgLs[i]) == pkg_list.end()) {
        pkg_list.push_back(this->_pkgLs[i]);
        ++n;
      }
    }
  }
  return n;
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
size_t OmLocation::bckGetExtras(vector<OmPackage*>& pkg_ls, vector<OmPackage*>& ovr_ls, vector<OmPackage*>& dep_ls, const OmPackage* pkg) const
{
  size_t n = 0;

  bool is_ovr;
  bool is_dep;

  for(size_t i = 0; i < this->_pkgLs.size(); ++i) {

    // search only among installed packages
    if(!this->_pkgLs[i]->hasBck())
      continue;

    // check both if package is overlapping and/or depend on
    // the currently specified one
    is_ovr = this->_pkgLs[i]->ovrHas(pkg->_hash);
    is_dep = this->_pkgLs[i]->depHas(pkg->_ident);

    if(is_ovr || is_dep) {

      // we go for recursive search to get a properly sorted list of
      // packages in depth-first search order.
      n += this->bckGetExtras(pkg_ls, ovr_ls, dep_ls, this->_pkgLs[i]);

      // we now add to the proper lists
      if(is_ovr) {
        // add only if unique
        if(std::find(ovr_ls.begin(), ovr_ls.end(), this->_pkgLs[i]) == ovr_ls.end()) {
          ovr_ls.push_back(this->_pkgLs[i]);
        }
      }

      if(is_dep) {
        // add only if unique
        if(std::find(dep_ls.begin(), dep_ls.end(), this->_pkgLs[i]) == dep_ls.end()) {
          dep_ls.push_back(this->_pkgLs[i]);
        }
      }

      // finally add to main list
      if(std::find(pkg_ls.begin(), pkg_ls.end(), this->_pkgLs[i]) == pkg_ls.end()) {
        pkg_ls.push_back(this->_pkgLs[i]);
        ++n;
      }
    }
  }

  return n;
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
bool OmLocation::repAdd(const wstring& base, const wstring& name)
{
  if(this->_config.valid()) {

    if(!this->_config.xml().hasChild(L"network")) {
      this->_config.xml().addChild(L"network");
    }

    OmXmlNode xml_net = this->_config.xml().child(L"network");

    // check whether repository already exists
    vector<OmXmlNode> xml_rep_list;
    xml_net.children(xml_rep_list, L"repository");

    for(size_t i = 0; i < xml_rep_list.size(); ++i) {
      if(base == xml_rep_list[i].attrAsString(L"base") && name == xml_rep_list[i].attrAsString(L"name")) {
        this->_error = L"Repository with same parameters already exists";
        this->log(1, L"Location("+this->_title+L") Add Repository", this->_error);
        return false;
      }
    }

    // add repository entry in definition
    OmXmlNode xml_rep = xml_net.addChild(L"repository");
    xml_rep.setAttr(L"base", base);
    xml_rep.setAttr(L"name", name);

    this->_config.save();

    // add repository in local list
    OmRepository* pRep = new OmRepository(this);

    // set repository parameters
    if(!pRep->init(base, name)) {
      this->log(1, L"Location("+this->_title+L") Add Repository", pRep->lastError());
      delete pRep;
      return false;
    }

    // add to list
    this->_repLs.push_back(pRep);
  }

  return true;
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
void OmLocation::repRem(unsigned id)
{
  if(this->_config.valid()) {

    OmRepository* pRep = this->_repLs[id];

    // remove repository from definition
    if(this->_config.xml().hasChild(L"network")) {
      OmXmlNode xml_net = this->_config.xml().child(L"network");

      // check whether repository already exists
      vector<OmXmlNode> xml_rep_list;
      xml_net.children(xml_rep_list, L"repository");

      for(size_t i = 0; i < xml_rep_list.size(); ++i) {
        if(pRep->base() == xml_rep_list[i].attrAsString(L"base") &&
           pRep->name() == xml_rep_list[i].attrAsString(L"name")) {
          xml_net.remChild(xml_rep_list[i]);
          break;
        }
      }
    }

    this->_config.save();

    // delete object
    delete pRep;

    // remove from local list
    this->_repLs.erase(this->_repLs.begin()+id);
  }
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
bool OmLocation::repQuery(Om_progressCb progress_cb, void* user_ptr)
{
  // check whether we have something to proceed
  if(this->_repLs.empty())
    return true;

  // initialize progression
  size_t progress_tot, progress_cur;
  if(progress_cb) {
    progress_tot = this->_repLs.size();
    progress_cur = 0;
    if(!progress_cb(user_ptr, progress_tot, progress_cur, nullptr))
      return true;
  }

  bool has_error = false;

  // clear remote package list
  this->_rmtLs.clear();

  this->log(2, L"Location("+this->_title+L") Network refresh", L"Querying "+to_wstring(this->_repLs.size())+L" repository.");

  unsigned c, n = 0;

  for(size_t i = 0; i < this->_repLs.size(); ++i) {

    if(this->_repLs[i]->query()) {
      n++;
    } else {
      this->_error =  L"Repository \""+this->_repLs[i]->url()+L"-"+this->_repLs[i]->name()+L"\" ";
      this->_error += L"query failed: " + this->_repLs[i]->lastError();
      this->log(1, L"Location("+this->_title+L") Network refresh", this->_error);
      has_error = true;
    }

    // add/merge remote packages to list
    c = this->_repLs[i]->rmtMerge(this->_rmtLs);
    this->log(2, L"Location("+this->_title+L") Network refresh", to_wstring(c)+L" new remote packages merged.");

    // call progression callback
    if(progress_cb) {
      progress_cur++;
      if(!progress_cb(user_ptr, progress_tot, progress_cur, this->_repLs[i]->url().c_str()))
        break;
    }
  }

  this->log(2, L"Location("+this->_title+L") Network refresh", to_wstring(n)+L" repository successfully parsed.");

  // force refresh
  this->rmtRefresh(true);

  return !has_error;
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
void OmLocation::rmtSetSorting(OmLocLsSort sorting)
{
  // we check if the requested sorting kind is the same as the currently
  // used, in this case, this mean the sorting order must be reversed

  if(sorting & this->_rmtSorting) {

    // check if current sorting is reversed, then switch order
    if(this->_rmtSorting & LS_SORT_REVERSE) {
      this->_rmtSorting &= ~LS_SORT_REVERSE; //< remove reversed flag
    } else {
      this->_rmtSorting |= LS_SORT_REVERSE; //< add reversed flag
    }

  } else {

    this->_rmtSorting = sorting;
  }

  // save the current sorting
  if(this->_config.valid()) {

    if(this->_config.xml().hasChild(L"remotes_sort")) {
      this->_config.xml().child(L"remotes_sort").setAttr(L"sort", static_cast<int>(this->_rmtSorting));
    } else {
      this->_config.xml().addChild(L"remotes_sort").setAttr(L"sort", static_cast<int>(this->_rmtSorting));
    }

    this->_config.save();
  }

  // finally sort packages
  this->_rmtSort();
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
bool OmLocation::rmtRefresh(bool force)
{
  bool changed = false;

  // refresh library first
  if(this->libRefresh() || force) {

    bool in_library;
    unsigned new_state;

    // compare remote package list to define remote status
    for(size_t r = 0; r < this->_rmtLs.size(); ++r) {

      new_state = this->_rmtLs[r]->_state;
      in_library = false;

      // remove relevant states, but not all of them to
      // keep states such as downloading or error
      new_state &= ~RMT_STATE_UPG;
      new_state &= ~RMT_STATE_DNG;
      new_state &= ~RMT_STATE_LOC;
      new_state &= ~RMT_STATE_NEW;

      for(size_t p = 0; p < this->_pkgLs.size(); ++p) {
        // search for same core but different version
        if(this->_rmtLs[r]->core() == this->_pkgLs[p]->core()) {
          // check whether this identity matches
          if(this->_rmtLs[r]->ident() != this->_pkgLs[p]->ident()) {
            // check version changes
            if(this->_rmtLs[r]->version() > this->_pkgLs[p]->version()) {
              new_state |= RMT_STATE_UPG;
              // add superseded package
              this->_rmtLs[r]->_supLs.push_back(this->_pkgLs[p]);
            } else {
              new_state |= RMT_STATE_DNG;
            }
          } else {
            // same identity, package already exists locally
            new_state |= RMT_STATE_LOC;
            in_library = true;
          }
          continue;
        }
      }

      if(!in_library) {
        new_state |= RMT_STATE_NEW;
      }

      if(new_state != this->_rmtLs[r]->_state) {
        this->_rmtLs[r]->_state = new_state;
        changed = true;
      }
    }
  }

  if(changed || force)
    this->_rmtSort();

  #ifdef DEBUG
  std::cout << "DEBUG => OmLocation::rmtRefresh " << (changed ? "+-" : "==") << "\n";
  #endif

  return changed;
}

///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
void OmLocation::rmtPrepareDown(vector<OmRemote*>& dnl_ls, vector<OmRemote*>& dep_ls, vector<wstring>& mis_ls, const vector<OmRemote*>& sel_ls) const
{
  // gather dependencies and create missing lists
  vector<wstring> idt_ls;
  for(size_t i = 0; i < sel_ls.size(); ++i) {

    idt_ls.clear();
    this->rmtGetDepends(dnl_ls, idt_ls, sel_ls[i]);

    for(size_t j = 0; j < idt_ls.size(); ++j) {
      // add uniques only
      if(std::find(mis_ls.begin(), mis_ls.end(), idt_ls[j]) == mis_ls.end()) {
        mis_ls.push_back(idt_ls[j]);
      }
    }
  }

  // create the extra install list
  for(size_t i = 0; i < dnl_ls.size(); ++i) {
    // add only if not in the initial selection
    if(std::find(sel_ls.begin(), sel_ls.end(), dnl_ls[i]) == sel_ls.end()) {
      dep_ls.push_back(dnl_ls[i]);
    }
  }

  // compose the final install list
  for(size_t i = 0; i < sel_ls.size(); ++i) {
    // add only if not already in install list
    if(std::find(dnl_ls.begin(), dnl_ls.end(), sel_ls[i]) == dnl_ls.end()) {
      dnl_ls.push_back(sel_ls[i]);
    }
  }
}

///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
size_t OmLocation::rmtGetDepends(vector<OmRemote*>& dep_ls, vector<wstring>& mis_ls, const OmRemote* rmt) const
{
  size_t n = 0;

  bool in_library;
  bool missing;

  for(size_t i = 0; i < rmt->depCount(); ++i) {

    in_library = false;

    // we first check if we found the dependency in the local package list
    for(size_t j = 0; j < this->_pkgLs.size(); ++j) {

      // rely only on packages
      if(!this->_pkgLs[j]->isZip())
        continue;

      if(rmt->depGet(i) == this->_pkgLs[j]->ident()) {
        in_library = true; break;
      }
    }

    // we found the dependency in library, we can go next candidate
    if(in_library) continue;

    missing = true;

    // now check if we found it in remote package list
    for(size_t j = 0; j < this->_rmtLs.size(); ++j) {

      if(rmt->depGet(i) == this->_rmtLs[j]->ident()) {

        n += this->rmtGetDepends(dep_ls, mis_ls, this->_rmtLs[j]);
        // we add to list only if unique and not already installed, this allow
        // us to get a consistent dependency list for a bunch of package by
        // calling this function for each package without clearing the list
        if(std::find(dep_ls.begin(), dep_ls.end(), this->_rmtLs[j]) == dep_ls.end()) {
          dep_ls.push_back(this->_rmtLs[j]);
          ++n;
        }

        missing = false;
        break;
      }
    }

    if(missing) {
      // we add to list only if unique
      if(std::find(mis_ls.begin(), mis_ls.end(), rmt->depGet(i)) == mis_ls.end()) {
        mis_ls.push_back(rmt->depGet(i));
      }
    }
  }

  return n;
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
void OmLocation::log(unsigned level, const wstring& head, const wstring& detail)
{
  wstring log_str = L"Context("; log_str.append(this->_context->title());
  log_str.append(L"):: "); log_str.append(head);

  this->_context->log(level, log_str, detail);
}



///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
inline void OmLocation::_pkgSort()
{
 if(this->_pkgSorting & LS_SORT_STAT) //< sorting by status
    std::sort(this->_pkgLs.begin(), this->_pkgLs.end(), __pkg_comp_stat_fn);

  if(this->_pkgSorting & LS_SORT_NAME) //< sorting by name (alphabetical order)
    std::sort(this->_pkgLs.begin(), this->_pkgLs.end(), __pkg_comp_name_fn);

  if(this->_pkgSorting & LS_SORT_VERS) //< sorting by version (ascending)
    std::sort(this->_pkgLs.begin(), this->_pkgLs.end(), __pkg_comp_vers_fn);

  // check whether we need a normal or reverse sorting
  if(this->_pkgSorting & LS_SORT_REVERSE) {
    std::reverse(this->_pkgLs.begin(), this->_pkgLs.end());
  }
}


///
///  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
///
inline void OmLocation::_rmtSort()
{
 if(this->_rmtSorting & LS_SORT_STAT) //< sorting by status
    std::sort(this->_rmtLs.begin(), this->_rmtLs.end(), __rmt_comp_stat_fn);

  if(this->_rmtSorting & LS_SORT_NAME) //< sorting by name (alphabetical order)
    std::sort(this->_rmtLs.begin(), this->_rmtLs.end(), __rmt_comp_name_fn);

  if(this->_rmtSorting & LS_SORT_VERS) //< sorting by version (ascending)
    std::sort(this->_rmtLs.begin(), this->_rmtLs.end(), __rmt_comp_vers_fn);

  if(this->_rmtSorting & LS_SORT_SIZE) //< sorting by version (ascending)
    std::sort(this->_rmtLs.begin(), this->_rmtLs.end(), __rmt_comp_size_fn);

  // check whether we need a normal or reverse sorting
  if(this->_rmtSorting & LS_SORT_REVERSE) {
    std::reverse(this->_rmtLs.begin(), this->_rmtLs.end());
  }
}
