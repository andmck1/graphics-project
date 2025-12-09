#pragma once

#include <optional>
#include <unordered_map>

// (Microsoft Copilot, 2025)
template <typename umapKeyType, typename umapValueType>
std::optional<std::reference_wrapper<const umapValueType>>
getValueFromUnorderedMap(
    const std::unordered_map<umapKeyType, umapValueType> &umap,
    const umapKeyType &umapKey
)
{
    auto it = umap.find(umapKey);
    if (it == umap.end())
        return std::nullopt;
    return it->second;
}
