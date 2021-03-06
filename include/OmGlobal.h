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

#ifndef OMGLOBAL_H
#define OMGLOBAL_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdint.h>
#include <wctype.h>
#include <locale>
#include <codecvt>

#define NTDDI_VERSION             0x06000000    // NTDDI_VISTA
#define WINVER                    0x0600        // _WIN32_WINNT_VISTA

#include "curl/curl.h"

#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <gdiplus.h>
#include <uxtheme.h>
#include <shellapi.h>

#define OMM_APP_NAME              L"Open Mod Manager"
#define OMM_APP_SHORT_NAME        L"OpenModMan"

#define OMM_APP_MAJ               0
#define OMM_APP_MIN               9
#define OMM_APP_REV               7
#ifdef _WIN64
  #define OMM_APP_ARCH            L"x64"
#else
  #define OMM_APP_ARCH            L"x86"
#endif
#define OMM_APP_DATE              L"July 2021"
#define OMM_APP_AUTHOR            L"Eric M."
#define OMM_APP_CONTRIB           L""
#define OMM_APP_C_YEAR            L"2021"
#define OMM_APP_GIT               L"https://github.com/sedenion/OpenModMan"
#define OMM_APP_URL               L"https://github.com/sedenion/OpenModMan"

#define OMM_CFG_SIGN_APP          L"Open_Mod_Manager_Main"
#define OMM_CFG_SIGN_CTX          L"Open_Mod_Manager_Context"
#define OMM_CFG_SIGN_LOC          L"Open_Mod_Manager_Location"
#define OMM_CFG_SIGN_PKG          L"Open_Mod_Manager_Package"
#define OMM_CFG_SIGN_BCK          L"Open_Mod_Manager_Backup"
#define OMM_CFG_SIGN_BAT          L"Open_Mod_Manager_Batch"
#define OMM_CFG_SIGN_REP          L"Open_Mod_Manager_Repository"

#define OMM_PKG_FILES_FILTER      L"Package File (*.zip,*.ozp)\0*.ZIP;*.OZP;\0"
#define OMM_BCK_FILES_FILTER      L"Backup File (*.zip,*.ozb)\0*.ZIP;*.OZB;\0"
#define OMM_IMG_FILES_FILTER      L"Image file (*.bmp,*.jpg,*.jpeg,*.gif,*.png)\0*.BMP;*.JPG;*.JPEG;*.GIF;*.PNG;\0BMP file (*.bmp)\0*.BMP;\0JPEG file (*.jpg,*.jpeg)\0*.JPG;*.JPEG\0PNG file (*.png)\0*.PNG;\0CompuServe GIF (*.gif)\0*.GIF;\0"
#define OMM_XML_FILES_FILTER      L"XML File (*.xml)\0*.XML;\0"
#define OMM_TXT_FILES_FILTER      L"Text file (*.txt)\0*.TXT\0"
#define OMM_ICO_FILES_FILTER      L"Icon files (*.ico,*.exe)\0*.ICO;*.EXE;\0Programs (*.exe)\0*.EXE;\0Icons (*.ico)\0*.ICO;\0"

#define OMM_CTX_DEF_FILE_FILER    L"Context definition (*.omc)\0*.OMC;\0"
#define OMM_LOC_DEF_FILE_FILER    L"Location definition (*.oml)\0*.OML;\0"
#define OMM_PKG_DEF_FILE_FILER    L"Package definition (*.omp)\0*.OMP;\0"
#define OMM_BCK_DEF_FILE_FILER    L"Backup definition (*.omk)\0*.OMK;\0"
#define OMM_BAT_DEF_FILE_FILER    L"Batch definition (*.omb)\0*.OMB;\0"

#define OMM_CTX_DEF_FILE_EXT      L"omc"
#define OMM_LOC_DEF_FILE_EXT      L"oml"
#define OMM_BAT_DEF_FILE_EXT      L"omb"
#define OMM_PKG_DEF_FILE_EXT      L"omp"
#define OMM_BCK_DEF_FILE_EXT      L"omk"
#define OMM_PKG_FILE_EXT          L"ozp"
#define OMM_BCK_FILE_EXT          L"ozb"

#define OMM_IMAGE_TYPE_BMP        1
#define OMM_IMAGE_TYPE_JPG        2
#define OMM_IMAGE_TYPE_PNG        3
#define OMM_IMAGE_TYPE_GIF        4

#define OMM_PKG_THMB_SIZE         128

#define OMM_MAX_PATH              1024
#define OMM_ITM_BUFF              256

// sleep value for debug slow mode
#define OMM_DEBUG_SLOW            50 //< ms

using namespace std;

/// \brief Progress callback.
///
/// Generic callback function for process progression.
///
/// \param[in]  ptr   : User data pointer.
/// \param[in]  tot   : Number total count of items.
/// \param[in]  cur   : Number processed item count.
/// \param[in]  data  : Custom internal opaque data.
///
/// \return True to continue, false to abort process.
///
typedef bool (*Om_progressCb)(void* ptr, size_t tot, size_t cur, uint64_t data);

/// \brief Progress callback.
///
/// Generic callback function for process progression.
///
/// \param[in]  ptr   : User data pointer.
/// \param[in]  tot   : Total bytes to download.
/// \param[in]  cur   : Bytes downloaded.
/// \param[in]  spd   : Download speed.
/// \param[in]  data  : Custom internal opaque data.
///
/// \return True to continue, false to abort process.
///
typedef bool (*Om_downloadCb)(void* ptr, double tot, double cur, double spd, uint64_t data);

/// \brief Compute Hash.
///
/// Calculates and returns 64 bits unsigned integer hash (xxHash) of the given
/// data chunk.
///
/// \param[in]  data    : Data to compute Hash.
/// \param[in]  size    : Size of data in bytes.
///
/// \return Resulting 64 bits unsigned integer hash.
///
uint64_t Om_getXXHash3(const void* data, size_t size);

/// \brief Compute Hash.
///
/// Calculates and returns 64 bits unsigned integer hash (xxHash) of the given
/// wide string.
///
/// \param[in]  str    : Wide string to compute Hash.
///
/// \return Resulting 64 bits unsigned integer hash.
///
uint64_t Om_getXXHash3(const wstring& str);

/// \brief Get file checksum.
///
/// Calculates and returns the 16 characters hash string of the
/// given data.
///
/// \param[in]  path    : Path to file to generate checksum.
///
/// \return 16 hexadecimal characters hash string.
///
wstring Om_getChecksum(const wstring& path);

/// \brief Get file checksum.
///
/// Calculates the 16 characters hash string of the given data.
///
/// \param[in]  hex     : String to set as checksum string.
/// \param[in]  path    : Path to file to generate checksum.
///
/// \return True if operation succeed, false otherwise
///
bool Om_getChecksum(wstring& hex, const wstring& path);

/// \brief Compare file checksum.
///
/// Calculates the 16 characters hash string of the given data.
///
/// \param[in]  path    : Path to file to generate checksum.
/// \param[in]  hex     : Checksum hexadecimal string to compare.
///
/// \return true if checksum matches, false otherwise
///
bool Om_cmpChecksum(const wstring& path, const wstring& hex);

/// \brief Calculate CRC64 value.
///
/// Calculates and returns the CRC64 unsigned integer value of the given
/// data chunk.
///
/// \param[in]  data    : Data chunk to create CRC from.
/// \param[in]  size    : Size of data.
///
/// \return Resulting CRC64 unsigned integer.
///
uint64_t Om_getCRC64(const void* data, size_t size);

/// \brief Calculate CRC64 value.
///
/// Calculates and returns the CRC64 unsigned integer value of the given
/// wide string.
///
/// \param[in]  str     : Wide string to create CRC from.
///
/// \return Resulting CRC64 unsigned integer.
///
uint64_t Om_getCRC64(const wstring& str);

/// \brief Get string representation of a 64 bits integer.
///
/// Returns the hexadecimal representation of the given 64 bits unsigned
/// integer value as a wide string.
///
/// \param[in]  num     : 64 bits unsigned integer.
///
/// \return Hexadecimal string representation of 64 bits integer
///
inline wstring Om_toHexString(uint64_t num) {
  wchar_t num_buf[17];
  swprintf(num_buf, 17, L"%016llx", num);
  return wstring(num_buf);
}

/// \brief Get string representation of a 64 bits integer.
///
/// Set the specified wind string to the hexadecimal representation of the
/// given 64 bits unsigned integer value.
///
/// \param[out] str     : String to get result.
/// \param[in]  num     : 64 bits unsigned integer.
///
inline void Om_toHexString(wstring& str, uint64_t num) {
  wchar_t num_buf[17];
  swprintf(num_buf, 17, L"%016llx", num);
  str = num_buf;
}

/// \brief Get 64 bits integer from string.
///
/// Returns the 64 bits unsigned integer value of the given hexadecimal number
/// string representation.
///
/// \param[in]  str     : Hexadecimal string represented number.
///
/// \return Converted 64 bits integer value.
///
inline uint64_t Om_toUint64(const wstring& str) {
  return wcstoull(str.c_str(), nullptr, 16);
}

/// \brief Encode bytes to Base64.
///
/// Encode the given binary data to Base64 string.
///
/// \param[in]  data    : Data to encode.
/// \param[in]  size    : data size in bytes.
///
/// \return String containing Base64 encoded data
///
wstring Om_toBase64(const uint8_t* data, size_t size);

/// \brief Encode bytes to Base64.
///
/// Encode the given binary data to Base64 string.
///
/// \param[out] b64     : String to get result.
/// \param[in]  data    : Data to encode.
/// \param[in]  size    : data size in bytes.
///
void Om_toBase64(wstring& b64, const uint8_t* data, size_t size);

/// \brief Decode Base64 to bytes.
///
/// Decode the given Base64 string to binary data.
///
/// \param[in]  size    : Pointer to receive decoded data size
/// \param[out] b64     : Base64 string to decode.
///
/// \return Pointer to decoded data.
///
uint8_t* Om_fromBase64(size_t* size, const wstring& b64);

/// \brief Format to Base64 encoded Data URI.
///
/// Format the given data to Base64 encoded Data URI.
///
/// \param[out] uri       : String to get result.
/// \param[in]  mime_type : Data URI media type to set.
/// \param[in]  charset   : Optional text charset to define.
/// \param[in]  data      : Data to encode.
/// \param[in]  size      : data size in bytes.
///
void Om_encodeDataUri(wstring& uri, const wstring& mime_type, const wstring& charset, const uint8_t* data, size_t size);

/// \brief Get data from Data URI.
///
/// Get decoded data from Data URI
///
/// \param[out] size      : Pointer to receive decoded data size
/// \param[out] mime_type : String to receive data type
/// \param[out] charset   : Text charset if any
/// \param[in]  uri       : Data URI string to parse
///
/// \return Pointer to decoded data.
///
uint8_t* Om_decodeDataUri(size_t* size, wstring& mime_type, wstring& charset, const wstring& uri);

/// \brief Get current time.
///
/// Get current time values based on system local time.
///
/// \param[out] t_sec   : Pointer to receive seconds
/// \param[out] t_min   : Pointer to receive minute
/// \param[out] t_hour  : Pointer to receive hour
///
void Om_getTime(int *t_sec, int *t_min, int *t_hour);

/// \brief Get current date.
///
/// Get current date values based on system local time.
///
/// \param[out] t_day   : Pointer to receive day of month
/// \param[out] t_mon   : Pointer to receive month
/// \param[out] t_year  : Pointer to receive year
///
void Om_getDate(int *t_day, int *t_mon, int *t_year);

/// \brief Generate random bytes.
///
/// Generate a random bytes sequence with values from 0 to 255 of
/// the desired length.
///
/// \param[out] dest    : Buffer to receive randomly generated sequence
/// \param[in]  size    : Buffer length
///
void Om_getRandBytes(uint8_t* dest, size_t size);

/// \brief Generate UUID.
///
/// Generate a random generated UUID version 4 string.
///
/// \return 36 characters UUID version 4 string.
///
wstring Om_genUUID();

/// \brief UTF-8 to UTF-16 conversion.
///
/// Convert the given UTF-8 multibyte string into UTF-16 wide char string.
///
/// \param[in]  utf8    : UTF-8 multibyte string to convert.
///
/// \return UTF-16 wide char string conversion result.
///
wstring Om_fromUtf8(const char* utf8);

/// \brief UTF-8 to UTF-16 conversion.
///
/// Convert the given UTF-8 multibyte string into UTF-16 wide char string.
///
/// \param[in]  wstr    : Wide char string to get conversion result.
/// \param[in]  utf8    : Pointer to null terminated UTF-8 char buffer to convert.
///
/// \return Count of read UTF-8 characters.
///
size_t Om_fromUtf8(wstring& wstr, const char* utf8);

/// \brief UTF-16 to UTF-8 conversion.
///
/// Convert the given UTF-16 wide char string into UTF-8 multibyte string.
///
/// \param[in]  utf8    : Pointer to char buffer to get conversion result.
/// \param[in]  len     : Size of the supplied char buffer.
/// \param[in]  wstr    : Wide char string to convert.
///
/// \return Count of written ANSI characters.
///
size_t Om_toUtf8(char* utf8, size_t len, const wstring& wstr);

/// \brief UTF-16 to UTF-8 conversion.
///
/// Convert the given UTF-16 wide char string into UTF-8 multibyte string.
///
/// \param[in]  wstr    : Wide char string to convert.
///
/// \return UTF-8 multibyte string conversion result.
///
string Om_toUtf8(const wstring& wstr);

/// \brief UTF-16 to UTF-8 conversion.
///
/// Convert the given UTF-16 wide char string into UTF-8 multibyte string.
///
/// \param[in]  utf8    : Multibyte string to get conversion result.
/// \param[in]  wstr    : Wide char string to convert.
///
size_t Om_toUtf8(string& utf8, const wstring& wstr);

/// \brief Conversion to ANSI multibyte.
///
/// Convert the given wide char string into multibyte string using the
/// system default Windows ANSI code page.
///
/// \param[in]  ansi    : Pointer to char buffer to get conversion result.
/// \param[in]  len     : Size of the supplied char buffer.
/// \param[in]  wstr    : Wide char string to convert.
///
/// \return Count of written ANSI characters.
///
size_t Om_toAnsiCp(char* ansi, size_t len, const wstring& wstr);

/// \brief Conversion to ANSI multibyte.
///
/// Convert the given wide char string into multibyte string using the
/// system default Windows ANSI code page.
///
/// \param[in]  ansi    : Multibyte string to get conversion result.
/// \param[in]  wstr    : Wide char string to convert.
///
/// \return Count of written ANSI characters.
///
size_t Om_toAnsiCp(string& ansi, const wstring& wstr);

/// \brief Conversion from ANSI multibyte.
///
/// Convert the given multibyte string into wide char string using the
/// system default Windows ANSI code page.
///
/// \param[in]  wstr    : Wide char string to get conversion result.
/// \param[in]  ansi    : Pointer to char buffer to convert.
///
/// \return Count of written wide characters.
///
size_t Om_fromAnsiCp(wstring& wstr, const char* ansi);

/// \brief Zip CDR entry to Windows path conversion.
///
/// Convert the given Unicode (UTF-8) Zip CDR entry into its corresponding
/// standard back-slash Windows path (UTF-16).
///
/// \param[in]  wstr    : Wide char string to get conversion result.
/// \param[in]  zcdr    : Pointer to null terminated UTF-8 char buffer to convert.
///
/// \return Count of read CDR entry characters.
///
size_t Om_fromZipCDR(wstring& wstr, const char* zcdr);

/// \brief Windows path to Zip CDR entry conversion.
///
/// Convert the given standard back-slash Windows path (UTF-16) into its
/// corresponding Unicode (UTF-8) Zip CDR entry.
///
/// \param[in]  zcdr    : Pointer to char buffer to get conversion result.
/// \param[in]  len     : Size of the supplied char buffer.
/// \param[in]  wstr    : Standard Windows path to convert.
///
/// \return Count of written CDR entry characters.
///
size_t Om_toZipCDR(char* zcdr, size_t len, const wstring& wstr);

/// \brief Windows path to Zip CDR entry conversion.
///
/// Convert the given standard back-slash Windows path (UTF-16) into its
/// corresponding Unicode (UTF-8) Zip CDR entry.
///
/// \param[in]  zcdr    : Multibyte string to get conversion result.
/// \param[in]  wstr    : Standard Windows path to convert.
///
/// \return Count of written CDR entry characters.
///
size_t Om_toZipCDR(string& zcdr, const wstring& wstr);

/// \brief Sort strings
///
/// Sorts the given array of strings in alphanumeric order.
///
/// \param[in]  string  : Pointer to vector array of wstring to sort.
///
void Om_sortStrings(vector<wstring>* strings);

/// \brief Check URL validity
///
/// Checks whether the given string is a valid HTTP(S) URL.
///
/// \param[in]  url     : URL to check.
///
/// \return True if the given string is a valid URL, false otherwise.
///
bool Om_isValidUrl(const wchar_t* url);

/// \brief Check URL validity
///
/// Checks whether the given string is a valid HTTP(S) URL.
///
/// \param[in]  url     : URL to check.
///
/// \return True if the given string is a valid URL, false otherwise.
///
bool Om_isValidUrl(const wstring& url);

/// \brief Check URL path validity
///
/// Checks whether the given string is a valid HTTP(S) URL path.
///
/// \param[in]  path    : Path to check.
///
/// \return True if the given string is a valid URL path, false otherwise.
///
bool Om_isValidUrlPath(const wchar_t* path);

/// \brief Check URL path validity
///
/// Checks whether the given string is a valid HTTP(S) URL path.
///
/// \param[in]  path    : Path to check.
///
/// \return True if the given string is a valid URL path, false otherwise.
///
bool Om_isValidUrlPath(const wstring& path);

/// \brief Check file name validity
///
/// Checks whether the given string is suitable for file name or
/// contains illegal character(s).
///
/// \param[in]  name    : File name to check.
///
/// \return True if the given string is suitable for file name, false otherwise.
///
bool Om_isValidName(const wchar_t* name);

/// \brief Check file name validity
///
/// Checks whether the given string is suitable for Windows file name or
/// contains illegal character(s).
///
/// \param[in]  name    : File name to check.
///
/// \return True if the given string is suitable for file name, false otherwise.
///
bool Om_isValidName(const wstring& name);

/// \brief Check file path validity
///
/// Checks whether the given string is suitable for file path or
/// contains illegal character(s).
///
/// \param[in]  path    : File path to check.
///
/// \return True if the given string is suitable for file path, false otherwise.
///
bool Om_isValidPath(const wchar_t* path);

/// \brief Check file path validity
///
/// Checks whether the given string is suitable for Windows file path or
/// contains illegal character(s).
///
/// \param[in]  path    : File path to check.
///
/// \return True if the given string is suitable for file path, false otherwise.
///
bool Om_isValidPath(const wstring& path);

/// \brief Extract file extension from path
///
/// Extract and returns the file extension part of the given full file path
/// or file name.
///
/// \param[in]  path    : Windows formated file path or name.
///
/// \return Extracted extension without the separating dot.
///
inline wstring Om_getExtensionPart(const wstring& path) {
  size_t d = path.find_last_of(L'.') + 1;
  return path.substr(d, wstring::npos);
}

/// \brief Extract file part from path
///
/// Extracts and returns the path part that follows the last
/// separator (backslash). Assuming the given path is a full file
/// path, the file name with its extension will be extracted.
///
/// \param[in]  path    : Windows formated full path.
///
/// \return Extracted path part.
///
inline wstring Om_getFilePart(const wstring& path){
  size_t s = path.find_last_of(L'\\') + 1;
  return path.substr(s, wstring::npos);
}

/// \brief Extract directory part from path
///
/// Extracts and returns the path part that precede the last
/// separator (backslash), including the last separator. Assuming the given
/// path is a full file path, the full path without the file name will be
/// extracted.
///
/// \param[in]  path    : Windows formated file path.
///
/// \return Extracted path part.
///
inline wstring Om_getDirPart(const wstring& uri){
  size_t e = uri.find_last_of(L'\\');
  return uri.substr(0, e);
}

/** Get file name (without extension) part of an URI.
 *
 * \param[in] uri     URI to extract name from.
 *
 * \return Extracted name of the given URI.
 */

/// \brief Extract file name without extension from path
///
/// Extracts and returns the file name, without its extension, from
/// the given full path.
///
/// \param[in]  path    : Windows formated file path.
///
/// \return Extracted file name without extension.
///
inline wstring Om_getNamePart(const wstring& uri){
  size_t s = uri.find_last_of(L'\\') + 1;
  size_t e = uri.find_last_of(L'.');
  return uri.substr(s, e-s);
}

/// \brief Check if names matches
///
/// Checks whether two strings are equals in case insensitive way.
///
/// \param[in]  left    : First string to test.
/// \param[in]  right   : Second string to test.
///
/// \return True if strings are same despite unmatched case, false otherwise.
///
inline bool Om_namesMatches(const wstring& left, const wstring& right)
{
  if(left.size() != right.size())
    return false;

  for(size_t i = 0; i < left.size(); ++i) {
    if(towupper(left[i]) != towupper(right[i]))
      return false;
  }

  return true;
}

/// \brief Check if names matches
///
/// Checks whether two strings are equals in case insensitive way.
///
/// \param[in]  left    : First string to test.
/// \param[in]  right   : Second string to test.
///
/// \return True if strings are same despite unmatched case, false otherwise.
///
inline bool Om_namesMatches(const wstring& left, const wchar_t* right)
{
  if(left.size() != wcslen(right))
    return false;

  for(size_t i = 0; i < left.size(); ++i) {
    if(towupper(left[i]) != towupper(right[i]))
      return false;
  }

  return true;
}

/// \brief Check if for file name extension matches
///
/// Checks whether the given file has the specified extension.
///
/// \param[in]  file    : file name or full path to test extension.
/// \param[in]  ext     : Three or four letters extension to check for.
///
/// \return True if given filename has the requested extension, false otherwise.
///
inline bool Om_extensionMatches(const wstring& file, const wchar_t* ext)
{
  size_t d = file.find_last_of(L'.') + 1;
  if(d > 0 && d != wstring::npos) {

    size_t len = wcslen(ext);

    if(len != (file.size() - d))
      return false;

    // compare case-insensitive
    for(size_t i = 0; i < len; ++i) {
      if(towupper(file[d+i]) != towupper(ext[i]))
        return false;
    }

    return true;
  } else {
    return false;
  }
}

/// \brief Concatenate paths
///
/// Concatenates two paths, adding separator if necessary.
///
/// \param[in]  left    : Left path part to concatenate.
/// \param[in]  right   : Right path part to concatenate.
///
/// \return Result of the concatenation.
///
inline wstring Om_concatPaths(const wstring& left, const wstring& right) {

  wstring result;
  if(left.empty()) {
    result = right;
  } else {
    result = left;
    if(left.back() != L'\\' && right.front() != L'\\') result += L"\\";
    result += right;
  }
  return result;
}

/// \brief Concatenate paths
///
/// Concatenates two paths, adding separator if necessary.
///
/// \param[out] conc    : String to be set as the result of concatenation.
/// \param[in]  left    : Left path part to concatenate.
/// \param[in]  right   : Right path part to concatenate.
///
inline void Om_concatPaths(wstring& conc, const wstring& left, const wstring& right) {

  if(left.empty()) {
    conc = right;
  } else {
    conc = left;
    if(left.back() != L'\\' && right.front() != L'\\') conc += L"\\";
    conc += right;
  }
}

/// \brief Compare path roots
///
/// Checks whether the given path has the specified root.
///
/// \param[in]  root    : Root path for verification.
/// \param[in]  item    : Path to verify if it has the specified root.
///
/// \return True if path actually has the specified root, false otherwise.
///
inline bool Om_isRootOfPath(const wstring& root, const wstring& path) {

  size_t l = root.size();

  if(l > path.size())
    return false;

  if(path.compare(0,l,root) == 0) {
    return (path[l] == L'\\'); //< verify this is a folder and not a file
  }

  return false;
}

/// \brief Get relative part of path
///
/// Checks whether the given path has the specified root, then extract its
/// relative part.
///
/// \param[out] rel     : String to be set as the extracted relative path.
/// \param[in]  root    : Root of path used to check and extract relative part.
/// \param[in]  item    : Path to be checked and to get the relative part from.
///
/// \return True if path actually has the specified root and operation succeed,
///         false otherwise.
///
inline bool Om_getRelativePath(wstring& rel, const wstring& root, const wstring& path) {

  size_t l = root.size();

  if(l > path.size())
    return false;

  if(path.compare(0,l,root) == 0) {
    if(path[l] == L'\\') { //< verify this is a folder
      l++;
      if(path.size() > l) {
        rel = path.substr(l, -1);
        return true;
      }
    }
  }

  return false;
}

/// \brief Get formated bytes/octets size string
///
/// Create a formated string of the given size in bytes in human readable
/// form with proper unit conversion.
///
/// \param[in]  bytes   : Size in bytes.
/// \param[in]  octet   : Write string using O for octet instead of B for bytes.
///
/// \return Formated string describing size.
///
wstring Om_formatSizeStr(size_t bytes, bool octet = false);

/// \brief Get system formated bytes size string
///
/// Create a formated string of the given size in bytes as system wide standard.
///
/// \param[in]  bytes   : Size in bytes.
/// \param[in]  kbytes  : Use kilobytes representation.
///
/// \return Formated string describing size.
///
wstring Om_formatSizeSysStr(size_t bytes, bool kbytes = true);

/// \brief Check whether is version string
///
/// Checks whether the given string can be parsed as valid version number(s).
///
/// \param[in] str       : String to test.
///
/// \return True if the given string can be parsed as version number(s).
///
bool Om_isVersionStr(const wstring& str);

/// \brief Parse Package filename
///
/// Parse the Packag display name and potential version substring from its
/// file name.
///
/// \param[out] name      : Parsed display name.
/// \param[out] core      : Parsed core name.
/// \param[out] vers      : Parsed version if any.
/// \param[in]  filename  : Filename to be parsed.
/// \param[in]  isfile    : Specify whether filename is file or a folder name.
/// \param[in]  us2spc    : Specify whether underscores must be replaced by spaces.
///
/// \return True if version string candidate was found, false otherwise
///
bool Om_parsePkgIdent(wstring& name, wstring& core, wstring& vers, const wstring& filename, bool isfile = true, bool us2spc = true);

/// \brief Check empty folder
///
/// Checks whether the specified folder is empty.
///
/// \param[in]  path   : Path to folder to check.
///
/// \return True if the specified folder is empty, false otherwise.
///
inline bool Om_isDirEmpty(const wstring& path) {
  return PathIsDirectoryEmptyW(path.c_str());
}

/// \brief Create folder
///
/// Creates the specified folder.
///
/// \param[in]  path   : Path of folder to create.
///
/// \return 0 if operation succeed, WinAPI error code otherwise.
///
inline int Om_dirCreate(const wstring& path) {
  if(!CreateDirectoryW(path.c_str(), nullptr)) {
    return GetLastError();
  }
  return 0;
}

/// \brief Create folder recursively
///
/// Creates the specified folder and its parent tree if needed.
///
/// \param[in]  path   : Path of folder(s) to create
///
/// \return 0 if operation succeed, WinAPI error code otherwise.
///
inline int Om_dirCreateRecursive(const wstring& path)
{
  return SHCreateDirectoryExW(nullptr, path.c_str(), nullptr);
}

/// \brief Delete folder
///
/// Deletes the specified folder.
///
/// \param[in]  path   : Path of folder to delete.
///
/// \return 0 if operation succeed, WinAPI error code otherwise.
///
inline int Om_dirDelete(const wstring& path) {
  if(!RemoveDirectoryW(path.c_str())) {
    return GetLastError();
  }
  return 0;
}

/// \brief Delete folder recursively
///
/// Deletes the specified folder and all its content recursively.
///
/// \param[in]  path   : Path of folder to delete.
///
/// \return 0 if operation succeed, WinAPI error code otherwise.
///
int Om_dirDeleteRecursive(const wstring& path);

/// \brief Copy file
///
/// Copy the given file to the specified location.
///
/// \param[in]  src    : Source file path to copy.
/// \param[in]  dst    : Destination file path.
/// \param[in]  ow     : Overwrite destination if exists.
///
/// \return 0 if operation succeed, WinAPI error code otherwise.
///
inline int Om_fileCopy(const wstring& src, const wstring& dst, bool ow = true) {
  if(!ow) {
    if(GetFileAttributesW(dst.c_str()) != INVALID_FILE_ATTRIBUTES)
      return 0; /* we do not write, but this is not a error */
  }
  if(!CopyFileW(src.c_str(),dst.c_str(), false)) {
    return GetLastError();
  }
  return 0;
}


/// \brief Copy file
///
/// Copy the given file to the specified location.
///
/// \param[in]  src    : Source file path to copy.
/// \param[in]  dst    : Destination file path.
///
/// \return 0 if operation succeed, WinAPI error code otherwise.
///
inline int Om_fileMove(const wstring& src, const wstring& dst) {
  if(!MoveFileExW(src.c_str(),dst.c_str(),MOVEFILE_REPLACE_EXISTING|MOVEFILE_COPY_ALLOWED|MOVEFILE_WRITE_THROUGH)) {
    return GetLastError();
  }
  return 0;
}


/// \brief Delete file
///
/// Delete the specified file.
///
/// \param[in]  path   : Path to file to delete.
///
/// \return 0 if operation succeed, WinAPI error code otherwise.
///
inline int Om_fileDelete(const wstring& path){
  if(!DeleteFileW(path.c_str())) {
    return GetLastError();
  }
  return 0;
}

/// \brief Check valid file
///
/// Checks whether the specified item is actually a valid file.
///
/// \param[in]  path   : Path to item to check.
///
/// \return True if item is actually a file, false if it does not exists or
///         is a folder.
///
inline bool Om_isFile(const wstring& path) {
  DWORD attr = GetFileAttributesW(path.c_str());
  if(attr != INVALID_FILE_ATTRIBUTES)
    return !(attr&FILE_ATTRIBUTE_DIRECTORY);
  return false;
}

/// \brief Check valid folder
///
/// Checks whether the specified item is actually a valid folder.
///
/// \param[in]  path   : Path to item to check.
///
/// \return True if item is actually a folder, false if it does not exists or
///         is a file.
///
inline bool Om_isDir(const wstring& path) {
  DWORD attr = GetFileAttributesW(path.c_str());
  if(attr != INVALID_FILE_ATTRIBUTES)
    return (attr&FILE_ATTRIBUTE_DIRECTORY);
  return false;
}

/// \brief Check existing item
///
/// Checks whether the specified item exists, either as file or folder.
///
/// \param[in]  path   : Path to item to check.
///
/// \return True if item is actually a file or folder, false otherwise.
///
inline bool Om_pathExists(const wstring& path) {
  return PathFileExistsW(path.c_str());
}

/// \brief Set item attribute
///
/// Set attribute to specified item, either file or folder.
///
/// \param[in]  path   : Path to item to set attribute.
/// \param[in]  attr   : Attribute to set.
///
/// \return True if operation succeed, false otherwise.
///
inline bool Om_itemSetAttr(const wstring& path, uint32_t attr) {
  return SetFileAttributesW(path.c_str(), attr);
}

/// \brief Check item attributes
///
/// Check whether item has the specified attribute mask, either file
/// or folder.
///
/// \param[in]  path   : Path to item to check attributes.
/// \param[in]  mask   : Attributes mask to check.
///
/// \return True if mask matches item attributes, false otherwise.
///
inline bool Om_itemHasAttr(const wstring& path, uint32_t mask) {
  DWORD attr = GetFileAttributesW(path.c_str());
  if(attr != INVALID_FILE_ATTRIBUTES)
    return ((mask & attr) == mask);
  return false;
}

/// \brief Check valid Zip file
///
/// Checks whether the specified item is file with Zip signature.
///
/// \param[in]  path   : Path to item to check.
///
/// \return True if item is actually a file with Zip signature, false otherwise.
///
bool Om_isFileZip(const wstring& path);

/// \brief List folders
///
/// Retrieves the list of folders contained in the specified origin location.
///
/// \param[out] ls      : Pointer to array of wstring to be filled with result.
/// \param[in]  origin  : Path where to list folder from.
/// \param[in]  abs     : If true, returns folder absolute path instead of
///                       folder name alone.
///
void Om_lsDir(vector<wstring>* ls, const wstring& origin, bool abs = true);

/// \brief List files
///
/// Retrieves the list of files contained in the specified origin location.
///
/// \param[out] ls      : Pointer to array of wstring to be filled with result.
/// \param[in]  origin  : Path where to list files from.
/// \param[in]  abs     : If true, returns files absolute path instead of
///                       files name alone.
///
void Om_lsFile(vector<wstring>* ls, const wstring& origin, bool abs = true);

/// \brief List files recursively
///
/// Retrieves the list of files contained in the specified origin
/// location, exploring sub-folders recursively.
///
/// \param[out] ls      : Pointer to array of wstring to be filled with result.
/// \param[in]  origin  : Path where to list items from.
/// \param[in]  abs     : If true, returns items absolute path instead of
///                       items name alone.
///
void Om_lsFileRecursive(vector<wstring>* ls, const wstring& origin, bool abs = true);

/// \brief List files with custom filter
///
/// Retrieves the list of files contained in the specified origin location using
/// the given custom filter.
///
/// \param[out] ls      : Pointer to array of wstring to be filled with result.
/// \param[in]  origin  : Path where to list files from.
/// \param[in]  filter  : Custom filter to select files.
/// \param[in]  abs     : If true, returns files absolute path instead of
///                       files name alone.
///
void Om_lsFileFiltered(vector<wstring>* ls, const wstring& origin, const wstring& filter, bool abs = true);

/// \brief List files and folders
///
/// Retrieves the list of files and folders contained in the specified origin
/// location.
///
/// \param[out] ls      : Pointer to array of wstring to be filled with result.
/// \param[in]  origin  : Path where to list items from.
/// \param[in]  abs     : If true, returns items absolute path instead of
///                       items name alone.
///
void Om_lsAll(vector<wstring>* ls, const wstring& origin, bool abs = true);

/// \brief List files and folders recursively
///
/// Retrieves the list of files and folders contained in the specified origin
/// location, exploring sub-folders recursively.
///
/// \param[out] ls      : Pointer to array of wstring to be filled with result.
/// \param[in]  origin  : Path where to list items from.
/// \param[in]  abs     : If true, returns items absolute path instead of
///                       items name alone.
///
void Om_lsAllRecursive(vector<wstring>* ls, const wstring& origin, bool abs = true);

/// \brief List folders and files with custom filter
///
/// Retrieves the list of folders and files contained in the specified origin location
/// using the given custom filter.
///
/// \param[out] ls      : Pointer to array of wstring to be filled with result.
/// \param[in]  origin  : Path where to list files from.
/// \param[in]  filter  : Custom filter to select files.
/// \param[in]  abs     : If true, returns files absolute path instead of
///                       files name alone.
///
void Om_lsAllFiltered(vector<wstring>* ls, const wstring& origin, const wstring& filter, bool abs = true);

/// \brief Get item total size
///
/// Retrieves the total size of the specified file or folder, including
/// all its content recursively.
///
/// \param[in]  path    : Item path.
///
/// \return Total size in bytes of the specified item
///
size_t Om_itemSize(const wstring& path);

/// \brief Get item last time
///
/// Retrieves the last write time of the specified file or folder.
///
/// \param[in]  path    : Item path.
///
/// \return Item last write time.
///
time_t Om_itemTime(const wstring& path);

/// \brief Move to trash
///
/// Moves the specified item to trash.
///
/// \param[in]  path    : Item path.
///
/// \return 0 if operation succeed, WinAPI error code otherwise.
///
int Om_moveToTrash(const wstring& path);

/// \brief Read into directory access mask
///
/// Access mask for directory read/traverse content
///
#define OMM_ACCESS_DIR_READ     FILE_LIST_DIRECTORY|FILE_TRAVERSE|FILE_READ_ATTRIBUTES

/// \brief Write into directory access mask
///
/// Access mask for directory write/add content
///
#define OMM_ACCESS_DIR_WRITE    FILE_ADD_FILE|FILE_ADD_SUBDIRECTORY|FILE_WRITE_ATTRIBUTES

/// \brief Execute file access mask
///
/// Access mask for fie execute
///
#define OMM_ACCESS_FILE_EXEC    FILE_EXECUTE

/// \brief Read file access mask
///
/// Access mask for file read data
///
#define OMM_ACCESS_FILE_READ    FILE_READ_DATA|FILE_READ_ATTRIBUTES

/// \brief Write file access mask
///
/// Access mask for file write/append data
///
#define OMM_ACCESS_FILE_WRITE   FILE_WRITE_DATA|FILE_APPEND_DATA|FILE_WRITE_ATTRIBUTES

/// \brief check file or directory permission
///
/// Checks whether the current process application have the specified
/// permissions on the given file or folder.
///
/// \param[in]  path  : Path to file or folder to check permission on.
/// \param[in]  mask  : Mask for requested permission.
///
/// \return True if requested permission are allowed, false otherwise
///
bool Om_checkAccess(const wstring& path, unsigned mask);

/// \brief Error message box.
///
/// Displays a standard Windows error message dialog box.
///
/// \param[in]  hwnd    : Parent window handle or nullptr to ignore.
/// \param[in]  header  : Formated message header/title (not the dialog title).
/// \param[in]  detail  : Formated message details paragraph.
///
void Om_dialogBoxErr(HWND hwnd, const wstring& header, const wstring& detail);

/// \brief Warning message box.
///
/// Displays a standard Windows warning message dialog box.
///
/// \param[in]  hwnd    : Parent window handle or nullptr to ignore.
/// \param[in]  header  : Formated message header/title (not the dialog title).
/// \param[in]  detail  : Formated message details paragraph.
///
void Om_dialogBoxWarn(HWND hwnd, const wstring& header, const wstring& detail);

/// \brief Notice message box.
///
/// Displays a standard Windows notice message dialog box.
///
/// \param[in]  hwnd    : Parent window handle or nullptr to ignore.
/// \param[in]  header  : Formated message header/title (not the dialog title).
/// \param[in]  detail  : Formated message details paragraph.
///
void Om_dialogBoxInfo(HWND hwnd, const wstring& header, const wstring& detail);

/// \brief Question dialog box.
///
/// Displays a standard Windows question dialog box with YES/NO buttons.
///
/// \param[in]  hwnd    : Parent window handle or nullptr to ignore.
/// \param[in]  header  : Formated message header/title (not the dialog title).
/// \param[in]  detail  : Formated message details paragraph.
///
/// \return True if user clicked on the YES button, false otherwise.
///
bool Om_dialogBoxQuerry(HWND hwnd, const wstring& header, const wstring& detail);

/// \brief Question warning dialog box.
///
/// Displays a standard Windows question with warning dialog box with
/// YES/NO buttons.
///
/// \param[in]  hwnd    : Parent window handle or nullptr to ignore.
/// \param[in]  header  : Formated message header/title (not the dialog title).
/// \param[in]  detail  : Formated message details paragraph.
///
/// \return True if user clicked on the YES button, false otherwise.
///
bool Om_dialogBoxQuerryWarn(HWND hwnd, const wstring& header, const wstring& detail);

/// \brief Select folder dialog box.
///
/// Opens a folder selection dialog box.
///
/// \param[out] result  : Buffer to receive the selected folder path.
/// \param[in]  hwnd    : Parent window handle or nullptr to ignore.
/// \param[in]  title   : Dialog window title.
/// \param[in]  start   : Path to default location where to begin browsing.
/// \param[in]  captive : If true, the default start location is set as root
///                       and user cannot browse up to parent folder.
///
/// \return True if succeed, false if user canceled or an error occurred.
///
bool Om_dialogBrowseDir(wstring& result, HWND hwnd, const wchar_t* title, const wstring& start, bool captive = false);

/// \brief Open file dialog box.
///
/// Opens a file open selection dialog box.
///
/// \param[out] result  : Buffer to receive the selected folder path.
/// \param[in]  hwnd    : Parent window handle or nullptr to ignore.
/// \param[in]  title   : Dialog window title.
/// \param[in]  filter  : File type and extension filter string.
/// \param[in]  start   : Path to default location where to begin browsing.
///
/// \return True if succeed, false if user canceled or an error occurred.
///
bool Om_dialogOpenFile(wstring& result, HWND hwnd, const wchar_t* title, const wchar_t* filter, const wstring& start);

/// \brief Save file dialog box.
///
/// Opens a file save as selection dialog box.
///
/// \param[out] result  : Buffer to receive the selected folder path.
/// \param[in]  hwnd    : Parent window handle or nullptr to ignore.
/// \param[in]  title   : Dialog window title.
/// \param[in]  filter  : File type and extension filter string.
/// \param[in]  start   : Path to default location where to begin browsing.
///
/// \return True if succeed, false if user canceled or an error occurred.
///
bool Om_dialogSaveFile(wstring& result, HWND hwnd, const wchar_t* title, const wchar_t* filter, const wstring& start);

/// \brief Create folder dialog process.
///
/// Checks whether specified path exists, if not ask the user to create
/// it and warn user if creation failed.
///
/// \param[in]  hwnd    : Dialog boxes parent window handle or nullptr to ignore.
/// \param[in]  item    : Formated message item to set in question message.
/// \param[in]  path    : Path to check and create if required.
///
/// \return True if succeed, false if user canceled or an error occurred.
///
bool Om_dialogCreateFolder(HWND hwnd, const wstring& item, const wstring& path);

/// \brief Overwrite file dialog process.
///
/// Checks whether specified file exists, if yes, asks user to overwrite
/// existing file.
///
/// \param[in]  hwnd      : Dialog boxes parent window handle or nullptr to ignore.
/// \param[in]  path      : File path to check.
///
/// \return True if succeed, false if user canceled or an error occurred.
///
bool Om_dialogOverwriteFile(HWND hwnd, const wstring& path);

/// \brief Unsaved changes before closing dialog.
///
/// Generic dialog message to ask for saving unsaved changes before closing.
///
/// \param[in]  hwnd    : Dialog boxes parent window handle or nullptr to ignore.
///
/// \return True if user choose to close anyway, false otherwise
///
bool Om_dialogCloseUnsaved(HWND hwnd);

/// \brief Unsaved changes before reset dialog.
///
/// Generic dialog message to ask for saving unsaved changes before reset.
///
/// \param[in]  hwnd    : Dialog boxes parent window handle or nullptr to ignore.
///
/// \return True if user choose to close anyway, false otherwise
///
bool Om_dialogResetUnsaved(HWND hwnd);

/// \brief File save success dialog.
///
/// Generic dialog message for file save success.
///
/// \param[in]  hwnd    : Dialog boxes parent window handle or nullptr to ignore.
/// \param[in]  item    : Formated message item to set in success message.
///
void Om_dialogSaveSucces(HWND hwnd, const wstring& item);

/// \brief File save error dialog.
///
/// Generic dialog message for file save error.
///
/// \param[in]  hwnd    : Dialog boxes parent window handle or nullptr to ignore.
/// \param[in]  item    : Formated message item to set in error message.
/// \param[in]  error   : Formated message error to set in error message.
///
void Om_dialogSaveError(HWND hwnd, const wstring& item, const wstring& error);

/// \brief Valid name dialog process.
///
/// Checks whether specified name is valid and if not warn user with proper
/// dialog box.
///
/// \param[in]  hwnd    : Dialog boxes parent window handle or nullptr to ignore.
/// \param[in]  name    : Formated message item to set as invalid.
/// \param[in]  name    : Name to check.
///
/// \return True if name pass validation, false otherwise.
///
bool Om_dialogValidName(HWND hwnd, const wstring& item,  const wstring& name);

/// \brief Valid path dialog process.
///
/// Checks whether specified name is valid and if not warn user with proper
/// dialog box.
///
/// \param[in]  hwnd    : Dialog boxes parent window handle or nullptr to ignore.
/// \param[in]  name    : Formated message item to set as invalid.
/// \param[in]  path    : Path to check.
///
/// \return True if name pass validation, false otherwise.
///
bool Om_dialogValidPath(HWND hwnd, const wstring& item,  const wstring& path);

/// \brief Valid directory dialog process.
///
/// Checks whether specified path is an existing directory and if not
/// warn user with proper dialog box.
///
/// \param[in]  hwnd    : Dialog boxes parent window handle or nullptr to ignore.
/// \param[in]  name    : Formated message item to set as invalid.
/// \param[in]  path    : Path to check.
///
/// \return True if path pass validation, false otherwise.
///
bool Om_dialogValidDir(HWND hwnd, const wstring& item,  const wstring& path);

/// \brief Load plan text.
///
/// Loads the specified file as plain text file.
///
/// \param[in] path    : Path to text file to be loaded.
///
/// \return String of file content
///
string Om_loadPlainText(const wstring& path);

/// \brief Load plan text.
///
/// Loads the specified file as plain text file.
///
/// \param[in] text    : String to receive loaded text data.
/// \param[in] path    : Path to text file to be loaded.
///
/// \return Count of bytes read.
///
size_t Om_loadPlainText(string& text, const wstring& path);

/// \brief Load binary file.
///
/// Loads the specified file as binary data.
///
/// \param[in] size    : Pointer to receive size of load data in bytes.
/// \param[in] path    : Path to file to be loaded.
///
/// \return Pointer to loaded data.
///
uint8_t* Om_loadBinary(size_t* size, const wstring& path);

/// \brief Get image type.
///
/// Search within the given data buffer for known image signature. Possible
/// values are the following:
///
/// \c 0 : unknown or invalid image format.
/// \c 1 or \c OMM_IMAGE_TYPE_BMP : BMP image.
/// \c 2 or \c OMM_IMAGE_TYPE_JPG : JPEG image.
/// \c 3 or \c OMM_IMAGE_TYPE_PNG : PNG image.
/// \c 4 or \c OMM_IMAGE_TYPE_GIF : GIF image.
///
/// \param[in]  data  : Image data to read, must be at least 8 bytes length.
///
/// \return Image type or 0 if unknown type.
///
int Om_imageType(uint8_t* data);

/// \brief Get image type.
///
/// Search within the given data buffer for known image signature. Possible
/// values are the following:
///
/// \c 0 : unknown or invalid image format.
/// \c 1 or \c OMM_IMAGE_TYPE_BMP : BMP image.
/// \c 2 or \c OMM_IMAGE_TYPE_JPG : JPEG image.
/// \c 3 or \c OMM_IMAGE_TYPE_PNG : PNG image.
/// \c 4 or \c OMM_IMAGE_TYPE_GIF : GIF image.
///
/// \param[in]  file  : File pointer to read data.
///
/// \return Image type, 0 if unknown type or -1 if read error occurred.
///
int Om_imageType(FILE* file);

/// \brief Load image.
///
/// Load image data from image file. Supported format are Bmp, Jpeg, Png and Gif.
///
/// \param[out] out_w   : Output image width
/// \param[out] out_h   : Output image height
/// \param[out] out_c   : Output image color component count.
/// \param[in]  in_path : Input image file path to read data.
/// \param[in]  flip_y  : Load image for bottom-left origin usage (upside down)
///
/// \return Pointer to RGB(A) image data or nullptr if failed.
///
uint8_t* Om_loadImage(unsigned* out_w, unsigned* out_h, unsigned* out_c, const wstring& in_path, bool flip_y = false);

/// \brief Load image.
///
/// Load image data from buffer in memory. Supported format are Bmp, Jpeg, Png and Gif.
///
/// \param[out] out_w   : Output image width
/// \param[out] out_h   : Output image height
/// \param[out] out_c   : Output image color component count.
/// \param[in]  in_data : Input image data to decode.
/// \param[in]  in_size : Input image data size in bytes.
/// \param[in]  flip_y  : Load image for bottom-left origin usage (upside down)
///
/// \return Pointer to RGB(A) image data or nullptr if failed.
///
uint8_t* Om_loadImage(unsigned* out_w, unsigned* out_h, unsigned* out_c, const uint8_t* in_data, size_t in_size, bool flip_y = false);

/// \brief Save image as BMP.
///
/// Save given image data as BMP file.
///
/// \param[out] out_path  : File path to save.
/// \param[in]  in_rgb    : Input image RGB(A) data to encode.
/// \param[in]  in_w      : Input image width.
/// \param[in]  in_h      : Input image height.
/// \param[in]  in_c      : Input image color component count, either 3 or 4.
///
/// \return True if operation succeed, false otherwise
///
bool Om_saveBmp(const wstring& out_path, const uint8_t* in_rgb, unsigned in_w, unsigned in_h, unsigned in_c);

/// \brief Save image as JPEG.
///
/// Save given image data as JPEG file.
///
/// \param[out] out_path  : File path to save.
/// \param[in]  in_rgb    : Input image RGB(A) data to encode.
/// \param[in]  in_w      : Input image width.
/// \param[in]  in_h      : Input image height.
/// \param[in]  in_c      : Input image color component count, either 3 or 4.
/// \param[in]  level     : JPEG compression quality 0 to 100.
///
/// \return True if operation succeed, false otherwise
///
bool Om_saveJpg(const wstring& out_path, const uint8_t* in_rgb, unsigned in_w, unsigned in_h, unsigned in_c, int level = 8);

/// \brief Save image as PNG.
///
/// Save given image data as PNG file.
///
/// \param[out] out_path  : File path to save.
/// \param[in]  in_rgb    : Input image RGB(A) data to encode.
/// \param[in]  in_w      : Input image width.
/// \param[in]  in_h      : Input image height.
/// \param[in]  in_c      : Input image color component count, either 3 or 4.
/// \param[in]  level     : PNG compression level 0 to 9.
///
/// \return True if operation succeed, false otherwise
///
bool Om_savePng(const wstring& out_path, const uint8_t* in_rgb, unsigned in_w, unsigned in_h, unsigned in_c, int level = 9);

/// \brief Save image as GIF.
///
/// Save given image data as GIF file.
///
/// \param[out] out_path  : File path to save.
/// \param[in]  in_rgb    : Input image RGB(A) data to encode.
/// \param[in]  in_w      : Input image width.
/// \param[in]  in_h      : Input image height.
/// \param[in]  in_c      : Input image color component count, either 3 or 4.
///
/// \return True if operation succeed, false otherwise
///
bool Om_saveGif(const wstring& out_path, const uint8_t* in_rgb, unsigned in_w, unsigned in_h, unsigned in_c);

/// \brief Encode BMP data.
///
/// Encode BMP data to buffer in memory.
///
/// \param[out] out_size  : Output BMP data size in bytes.
/// \param[in]  in_rgb    : Input image RGB(A) data to encode.
/// \param[in]  in_w      : Input image width.
/// \param[in]  in_h      : Input image height.
/// \param[in]  in_c      : Input image color component count, either 3 or 4.
///
/// \return Pointer to encoded BMP image data or nullptr if failed.
///
uint8_t* Om_encodeBmp(size_t* out_size, const uint8_t* in_rgb, unsigned in_w, unsigned in_h, unsigned in_c);

/// \brief Encode JPEG data.
///
/// Encode JPEG data to buffer in memory.
///
/// \param[out] out_data  : Output JPEG data, pointer to pointer to be allocated.
/// \param[out] out_size  : Output JPEG data size in bytes.
/// \param[in]  in_rgb    : Input image RGB(A) data to encode.
/// \param[in]  in_w      : Input image width.
/// \param[in]  in_h      : Input image height.
/// \param[in]  in_c      : Input image color component count, either 3 or 4.
/// \param[in]  level     : JPEG compression quality 0 to 100.
///
/// \return Pointer to encoded JPG image data or nullptr if failed.
///
uint8_t* Om_encodeJpg(size_t* out_size, const uint8_t* in_rgb, unsigned in_w, unsigned in_h, unsigned in_c, int level = 8);

/// \brief Encode PNG data.
///
/// Encode PNG data to buffer in memory.
///
/// \param[out] out_data  : Output PNG data, pointer to pointer to be allocated.
/// \param[out] out_size  : Output PNG data size in bytes.
/// \param[in]  in_rgb    : Input image RGB(A) data to encode.
/// \param[in]  in_w      : Input image width.
/// \param[in]  in_h      : Input image height.
/// \param[in]  in_c      : Input image color component count, either 3 or 4.
/// \param[in]  level     : PNG compression level 0 to 9.
///
/// \return Pointer to encoded PNG image data or nullptr if failed.
///
uint8_t* Om_encodePng(size_t* out_size, const uint8_t* in_rgb, unsigned in_w, unsigned in_h, unsigned in_c, int level = 9);

/// \brief Encode GIF data.
///
/// Encode GIF data to buffer in memory.
///
/// \param[out] out_data  : Output GIF data, pointer to pointer to be allocated.
/// \param[out] out_size  : Output GIF data size in bytes.
/// \param[in]  in_rgb    : Input image RGB(A) data to encode.
/// \param[in]  in_w      : Input image width.
/// \param[in]  in_h      : Input image height.
/// \param[in]  in_c      : Input image color component count, either 3 or 4.
///
/// \return Pointer to encoded GIF image data or nullptr if failed.
///
uint8_t* Om_encodeGif(size_t* out_size, const uint8_t* in_rgb, unsigned in_w, unsigned in_h, unsigned in_c);

/// \brief Resize image.
///
/// Resize given image data to the specified width and heigth.
///
/// \param[out] w         : Desired image width.
/// \param[out] h         : Desired image height.
/// \param[in]  in_rgb    : Input image RGB(A) data to encode.
/// \param[in]  in_w      : Input image width.
/// \param[in]  in_h      : Input image height.
/// \param[in]  in_c      : Input image color component count, either 3 or 4.
///
/// \return New pointer to resized image data or null if error.
///
uint8_t* Om_resizeImage(unsigned w, unsigned h, const uint8_t* in_rgb, unsigned in_w, unsigned in_h, unsigned in_c);

/// \brief Crop image.
///
/// Crop given image data according the specified rectangle coordinates.
///
/// \param[out] x         : Crop rectangle top-left corner horizontal position in image.
/// \param[out] y         : Crop rectangle top-left corner vertical position in image.
/// \param[out] w         : Crop rectangle width.
/// \param[out] h         : Crop rectangle height.
/// \param[in]  in_rgb    : Input image RGB(A) data to encode.
/// \param[in]  in_w      : Input image width.
/// \param[in]  in_h      : Input image height.
/// \param[in]  in_c      : Input image color component count, either 3 or 4.
///
/// \return New pointer to resized image data or null if error.
///
uint8_t* Om_cropImage(unsigned x, unsigned y, unsigned w, unsigned h, const uint8_t* in_rgb, unsigned in_w, unsigned in_h, unsigned in_c);

/// \brief Create image thumbnail.
///
/// Create thumbnail version of the given image data.
///
/// \param[out] size      : Thumbnail size.
/// \param[in]  in_rgb    : Input image RGB(A) data to encode.
/// \param[in]  in_w      : Input image width.
/// \param[in]  in_h      : Input image height.
/// \param[in]  in_c      : Input image color component count, either 3 or 4.
///
/// \return New pointer to resized image data or null if error.
///
uint8_t* Om_thumbnailImage(unsigned size, const uint8_t* in_rgb, unsigned in_w, unsigned in_h, unsigned in_c);

/// \brief Create HBITMAP image.
///
/// Create HBITMAP version of the given image data.
///
/// \param[in]  in_rgb    : Input image RGB(A) data to encode.
/// \param[in]  in_w      : Input image width.
/// \param[in]  in_h      : Input image height.
/// \param[in]  in_c      : Input image color component count, either 3 or 4.
///
/// \return New HBITMAP or null if error.
///
HBITMAP Om_hbitmapImage(const uint8_t* in_rgb, unsigned in_w, unsigned in_h, unsigned in_c);

/// \brief Load stock shell icon
///
/// Loads and returns the specified Windows Shell Stock icon.
///
/// \param[in] id      : Stock icon id, form SHSTOCKICONID enum
/// \param[in] large   : If true, load the large size icon version
///
/// \return Icon handle (HICON) of the specified Shell Stock icon
///
HICON Om_getShellIcon(unsigned id, bool large = false);

/// \brief Load stock shell icon as HBITMAP
///
/// Loads and returns the specified Windows Shell Stock icon as HBITMAP.
///
/// \param[in] id      : Stock icon id, form SHSTOCKICONID enum
/// \param[in] large   : If true, load the large size icon version
///
/// \return Bitmap handle (HBITMAP) of the specified Shell Stock icon
///
HBITMAP Om_loadShellBitmap(unsigned id, bool large = false);

/// \brief Get internal resource image
///
/// Return internal resource image as HBITMAP, created handle does not
/// have to be deleted.
///
/// \param[in] hins    : Handle instance to get internal resource.
/// \param[in] id      : Image internal resource id.
///
/// \return Bitmap handle (HBITMAP) of the internal image or nullptr.
///
HBITMAP Om_getResImage(HINSTANCE hins, unsigned id);

/// \brief Get internal resource icon
///
/// Return internal resource icon as HICON, created handle does not
/// have to be deleted.
///
/// \param[in] hins    : Handle instance to get internal resource.
/// \param[in] id      : Image internal resource id.
/// \param[in] size    : Icon size to get.
///
/// \return Bitmap handle (HBITMAP) of the internal image or nullptr.
///
HICON Om_getResIcon(HINSTANCE hins, unsigned id, unsigned size = 0);

/// \brief Create font object
///
/// Create and returns a font object according the specified parameters.
///
/// \param[in] pt      : Font size
/// \param[in] weight  : Font weight
/// \param[in] style   : Font style
/// \param[in] name    : Font name
///
/// \return Font handle (HFONT) of the created font
///
HFONT Om_createFont(unsigned pt, unsigned weight, const wchar_t* name);

/// \brief Compress data
///
/// Compress the supplied data using Deflate algorithm.
///
/// \param[out] out_size  : Output compressed data size in bytes.
/// \param[in]  in_data   : Input data to be compressed.
/// \param[in]  in_size   : Input data size in bytes.
/// \param[in]  level     : Deflate compression level 0 to 9.
///
/// \return Pointer to compressed data or nullptr if failed.
///
uint8_t* Om_zDeflate(size_t* out_size, const uint8_t* in_data, size_t in_size, unsigned level = 9);

/// \brief Uncompress data
///
/// Uncompress the supplied data using Deflate algorithm.
///
/// \param[in]  in_data   : Input data to be decompressed.
/// \param[in]  in_size   : Input data size in bytes.
/// \param[in]  def_size  : Original size of decompressed data (must have been stored before compression)
///
/// \return Pointer to uncompressed data or nullptr if failed.
///
uint8_t* Om_zInflate(const uint8_t* in_data, size_t in_size, size_t def_size);

/// \brief Get Windows error string
///
/// Returns the error string corresponding to the given Windows error code.
///
/// \param[in] code    : Windows error code.
///
/// \return Error string.
///
wstring Om_getErrorStr(int code);

/// \brief Invalid directory error message.
///
/// Compose error string for invalid or non existing directory.
///
/// \param[in]  name    : Formated message item designation.
/// \param[in]  path    : Formated message item path.
///
/// \return Formated error log message.
///
wstring Om_errIsDir(const wstring& item,  const wstring& path);

/// \brief Create error message.
///
/// Compose error string for unable to create.
///
/// \param[in]  name    : Formated message item designation.
/// \param[in]  path    : Formated message item path.
/// \param[in]  result  : WinAPI standard result error code.
///
/// \return Formated error log message.
///
wstring Om_errCreate(const wstring& item,  const wstring& path, int result);

/// \brief Delete error message.
///
/// Compose error string for unable to delete.
///
/// \param[in]  name    : Formated message item designation.
/// \param[in]  path    : Formated message item path.
/// \param[in]  result  : WinAPI standard result error code.
///
/// \return Formated error log message.
///
wstring Om_errDelete(const wstring& item,  const wstring& path, int result);

/// \brief Rename error message.
///
/// Compose error string for unable to rename.
///
/// \param[in]  name    : Formated message item designation.
/// \param[in]  path    : Formated message item path.
/// \param[in]  result  : WinAPI standard result error code.
///
/// \return Formated error log message.
///
wstring Om_errRename(const wstring& item, const wstring& path, int result);

/// \brief Move error message.
///
/// Compose error string for unable to move.
///
/// \param[in]  name    : Formated message item designation.
/// \param[in]  path    : Formated message item path.
/// \param[in]  result  : WinAPI standard result error code.
///
/// \return Formated error log message.
///
wstring Om_errMove(const wstring& item, const wstring& path, int result);

/// \brief Copy error message.
///
/// Compose error string for unable to copy.
///
/// \param[in]  name    : Formated message item designation.
/// \param[in]  path    : Formated message item path.
/// \param[in]  result  : WinAPI standard result error code.
///
/// \return Formated error log message.
///
wstring Om_errCopy(const wstring& item, const wstring& path, int result);

/// \brief Read access error message.
///
/// Compose error string for unable to read access.
///
/// \param[in]  name    : Formated message item designation.
/// \param[in]  path    : Formated message item path.
///
/// \return Formated error log message.
///
wstring Om_errReadAccess(const wstring& item, const wstring& path);

/// \brief Write access error message.
///
/// Compose error string for unable to write access.
///
/// \param[in]  name    : Formated message item designation.
/// \param[in]  path    : Formated message item path.
///
/// \return Formated error log message.
///
wstring Om_errWriteAccess(const wstring& item, const wstring& path);

/// \brief Zip init error message.
///
/// Compose error string for unable to init zip.
///
/// \param[in]  name    : Formated message item designation.
/// \param[in]  path    : Formated message item path.
/// \param[in]  mesg    : Additional error message.
///
/// \return Formated error log message.
///
wstring Om_errZipInit(const wstring& item, const wstring& path, const wstring& mesg);

/// \brief Zip open error message.
///
/// Compose error string for unable to open zip.
///
/// \param[in]  name    : Formated message item designation.
/// \param[in]  path    : Formated message item path.
/// \param[in]  mesg    : Additional error message.
///
/// \return Formated error log message.
///
wstring Om_errZipOpen(const wstring& item, const wstring& path, const wstring& mesg);

/// \brief Zip deflate error message.
///
/// Compose error string for unable to deflate file.
///
/// \param[in]  name    : Formated message item designation.
/// \param[in]  path    : Formated message item path.
/// \param[in]  mesg    : Additional error message.
///
/// \return Formated error log message.
///
wstring Om_errZipDefl(const wstring& item, const wstring& path, const wstring& mesg);

/// \brief Zip inflate error message.
///
/// Compose error string for unable to inflate file.
///
/// \param[in]  name    : Formated message item designation.
/// \param[in]  path    : Formated message item path.
/// \param[in]  mesg    : Additional error message.
///
/// \return Formated error log message.
///
wstring Om_errZipInfl(const wstring& item, const wstring& path, const wstring& mesg);

/// \brief Definition init error message.
///
/// Compose error string for unable to init definition.
///
/// \param[in]  name    : Formated message item designation.
/// \param[in]  path    : Formated message item path.
/// \param[in]  mesg    : Additional error message.
///
/// \return Formated error log message.
///
wstring Om_errDefInit(const wstring& item, const wstring& path, const wstring& mesg);

/// \brief Definition open error message.
///
/// Compose error string for unable to open definition.
///
/// \param[in]  name    : Formated message item designation.
/// \param[in]  path    : Formated message item path.
/// \param[in]  mesg    : Additional error message.
///
/// \return Formated error log message.
///
wstring Om_errDefOpen(const wstring& item, const wstring& path, const wstring& mesg);

/// \brief Definition parse error message.
///
/// Compose error string for unable to parse definition.
///
/// \param[in]  name    : Formated message item designation.
/// \param[in]  path    : Formated message item path.
/// \param[in]  mesg    : Additional error message.
///
/// \return Formated error log message.
///
wstring Om_errDefSave(const wstring& item, const wstring& path, const wstring& mesg);

/// \brief Memory allocation.
///
/// Allocate new buffer of the specified size. Allocated data
/// must be free using the Om_free function.
///
/// \param[in]  size  : Size to allocate in bytes.
///
/// \return Pointer to allocated buffer or nullptr if failed.
///
inline void* Om_alloc(size_t size) {
  return malloc(size);
}

/// \brief Reallocate memory.
///
/// Changes the size of the given memory block to the
/// specified size.
///
/// \param[in] buff : Pointer to a memory block previously allocated with Om_alloc or Om_realloc.
/// \param[in] size : New size for the memory block, in bytes.
///
/// \return Pointer to allocated buffer or nullptr if failed.
///
inline void* Om_realloc(void* buff, size_t size) {
  return realloc(buff, size);
}

/// \brief Free allocated data.
///
/// Properly free a previously allocated data using Om_alloc.
///
/// \param[in]  data  : Previously allocated to free.
///
inline void Om_free(void* data) {
  if(data) free(data);
}

#endif // OMGLOBAL_H
