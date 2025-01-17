//
// Copyright 2021 Autodesk
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#ifndef MAYAUSD_UTILS_UTILSERIALIZATION_H
#define MAYAUSD_UTILS_UTILSERIALIZATION_H

#include <mayaUsd/base/api.h>
#include <mayaUsd/nodes/proxyShapeBase.h>

#include <pxr/pxr.h>
#include <pxr/usd/sdf/fileFormat.h>
#include <pxr/usd/sdf/layer.h>

/// General utility functions used when serializing Usd edits during a save operation
namespace MAYAUSD_NS_DEF {
namespace utils {

/*! \brief Helps suggest a folder to export anonymous layers to.  Checks in order:
    1. File-backed root layer folder.
    2. Current Maya scene folder.
    3. Current Maya workspace scenes folder.
 */
MAYAUSD_CORE_PUBLIC
std::string suggestedStartFolder(PXR_NS::UsdStageRefPtr stage);

/*! \brief Queries Maya for the current workspace "scenes" folder.
 */
MAYAUSD_CORE_PUBLIC
std::string getSceneFolder();

MAYAUSD_CORE_PUBLIC
std::string generateUniqueFileName(const std::string& basename);

/*! \brief Queries the Maya optionVar that decides what the internal format
    of a .usd file should be, either "usdc" or "usda".
 */
MAYAUSD_CORE_PUBLIC
std::string usdFormatArgOption();

enum USDUnsavedEditsOption
{
    kSaveToUSDFiles = 1,
    kSaveToMayaSceneFile,
    kIgnoreUSDEdits
};
/*! \brief Queries the Maya optionVar that decides which saving option Maya
    should use for Usd edits.
 */
MAYAUSD_CORE_PUBLIC
USDUnsavedEditsOption serializeUsdEditsLocationOption();

/*! \brief Utility function to update the file path attribute on the proxy shape
    when an anonymous root layer gets exported to disk.
 */
MAYAUSD_CORE_PUBLIC
void setNewProxyPath(const MString& proxyNodeName, const MString& newValue);

struct LayerParent
{
    // Every layer that we are saving should have either a parent layer that
    // we will need to remap to point to the new path, or the stage if it is an
    // anonymous root layer.
    SdfLayerRefPtr _layerParent;
    std::string    _proxyPath;
};

struct stageLayersToSave
{
    std::vector<std::pair<SdfLayerRefPtr, LayerParent>> _anonLayers;
    std::vector<SdfLayerRefPtr>                         _dirtyFileBackedLayers;
};

/*! \brief Save an layer to disk to the given file path and using the given format.
    If the file path is empty then use the current file path of the layer.
    If the format is empty then use the current user-selected USD format option
    as defined by the usdFormatArgOption() function. (See above.)
 */
MAYAUSD_CORE_PUBLIC
bool saveLayerWithFormat(
    SdfLayerRefPtr     layer,
    const std::string& requestedFilePath = "",
    const std::string& requestedFormatArg = "");

/*! \brief Save an anonymous layer to disk and update the sublayer path array
    in the parent layer.
 */
MAYAUSD_CORE_PUBLIC
PXR_NS::SdfLayerRefPtr saveAnonymousLayer(
    PXR_NS::SdfLayerRefPtr anonLayer,
    LayerParent            parent,
    const std::string&     basename,
    std::string            formatArg = "");

/*! \brief Save an anonymous layer to disk and update the sublayer path array
    in the parent layer.
 */
MAYAUSD_CORE_PUBLIC
PXR_NS::SdfLayerRefPtr saveAnonymousLayer(
    PXR_NS::SdfLayerRefPtr anonLayer,
    const std::string&     path,
    bool                   savePathAsRelative,
    LayerParent            parent,
    std::string            formatArg = "");

/*! \brief Ensures that the filepath contains a valid USD extension.
 */
MAYAUSD_CORE_PUBLIC
void ensureUSDFileExtension(std::string& filePath);

/*! \brief Check the sublayer stack of the stage looking for any anonymous
    layers that will need to be saved.
 */
MAYAUSD_CORE_PUBLIC
void getLayersToSaveFromProxy(const std::string& proxyPath, stageLayersToSave& layersInfo);

} // namespace utils
} // namespace MAYAUSD_NS_DEF

#endif // MAYAUSD_UTILS_UTILSERIALIZATION_H
