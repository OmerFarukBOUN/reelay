#include <flatbuffers/flatbuffers.h>
namespace fbs
{
#include "osi_lane_generated.h"
}

#include "osi_lane.pb.h"

#include <vector>

#include "osi_common_converter.hpp"

// ==========================
// Lane - Classification - RoadCondition
// ==========================

::flatbuffers::Offset<fbs::osi3::Lane_::Classification_::RoadCondition> ConvertLaneClassificationRoadCondition(const osi3::Lane::Classification::RoadCondition& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::Lane_::Classification_::RoadConditionBuilder b(builder);
    
    if (proto.has_surface_temperature())
    	b.add_surface_temperature(proto.surface_temperature());
    
    if (proto.has_surface_water_film())
    	b.add_surface_water_film(proto.surface_water_film());
    
    if (proto.has_surface_freezing_point())
    	b.add_surface_freezing_point(proto.surface_freezing_point());
    
    if (proto.has_surface_ice())
    	b.add_surface_ice(proto.surface_ice());
    
    if (proto.has_surface_roughness())
    	b.add_surface_roughness(proto.surface_roughness());
    
    if (proto.has_surface_texture())
    	b.add_surface_texture(proto.surface_texture());
    
    return b.Finish();
}

// ==========================
// Lane - Classification - LanePairing
// ==========================

::flatbuffers::Offset<fbs::osi3::Lane_::Classification_::LanePairing> ConvertLaneClassificationLanePairing(const osi3::Lane::Classification::LanePairing& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::Lane_::Classification_::LanePairingBuilder b(builder);
    
    if (proto.has_antecessor_lane_id())
    	b.add_antecessor_lane_id(ConvertIdentifier(proto.antecessor_lane_id(), builder));
    
    if (proto.has_successor_lane_id())
    	b.add_successor_lane_id(ConvertIdentifier(proto.successor_lane_id(), builder));
    
    return b.Finish();
}

// ==========================
// Lane - Classification
// ==========================

::flatbuffers::Offset<fbs::osi3::Lane_::Classification> ConvertLaneClassification(const osi3::Lane::Classification& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::Lane_::ClassificationBuilder b(builder);
    
    if (proto.has_type())
    	b.add_type(static_cast<fbs::osi3::Lane_::Classification_::Type>(proto.type()));
    
    if (proto.has_is_host_vehicle_lane())
    	b.add_is_host_vehicle_lane(proto.is_host_vehicle_lane());
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Vector3d>> centerline;
    for (const auto& item : proto.centerline())
        centerline.push_back(ConvertVector3d(item, builder));
    b.add_centerline(builder.CreateVector(centerline));
    
    if (proto.has_centerline_is_driving_direction())
    	b.add_centerline_is_driving_direction(proto.centerline_is_driving_direction());
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Identifier>> left_adjacent_lane_id;
    for (const auto& item : proto.left_adjacent_lane_id())
        left_adjacent_lane_id.push_back(ConvertIdentifier(item, builder));
    b.add_left_adjacent_lane_id(builder.CreateVector(left_adjacent_lane_id));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Identifier>> right_adjacent_lane_id;
    for (const auto& item : proto.right_adjacent_lane_id())
        right_adjacent_lane_id.push_back(ConvertIdentifier(item, builder));
    b.add_right_adjacent_lane_id(builder.CreateVector(right_adjacent_lane_id));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Lane_::Classification_::LanePairing>> lane_pairing;
    for (const auto& item : proto.lane_pairing())
        lane_pairing.push_back(ConvertLaneClassificationLanePairing(item, builder));
    b.add_lane_pairing(builder.CreateVector(lane_pairing));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Identifier>> right_lane_boundary_id;
    for (const auto& item : proto.right_lane_boundary_id())
        right_lane_boundary_id.push_back(ConvertIdentifier(item, builder));
    b.add_right_lane_boundary_id(builder.CreateVector(right_lane_boundary_id));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Identifier>> left_lane_boundary_id;
    for (const auto& item : proto.left_lane_boundary_id())
        left_lane_boundary_id.push_back(ConvertIdentifier(item, builder));
    b.add_left_lane_boundary_id(builder.CreateVector(left_lane_boundary_id));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Identifier>> free_lane_boundary_id;
    for (const auto& item : proto.free_lane_boundary_id())
        free_lane_boundary_id.push_back(ConvertIdentifier(item, builder));
    b.add_free_lane_boundary_id(builder.CreateVector(free_lane_boundary_id));
    
    if (proto.has_road_condition())
    	b.add_road_condition(ConvertLaneClassificationRoadCondition(proto.road_condition(), builder));
    
    if (proto.has_subtype())
    	b.add_subtype(static_cast<fbs::osi3::Lane_::Classification_::Subtype>(proto.subtype()));
    
    return b.Finish();
}

// ==========================
// Lane
// ==========================

::flatbuffers::Offset<fbs::osi3::Lane> ConvertLane(const osi3::Lane& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::LaneBuilder b(builder);
    
    if (proto.has_id())
    	b.add_id(ConvertIdentifier(proto.id(), builder));
    
    if (proto.has_classification())
    	b.add_classification(ConvertLaneClassification(proto.classification(), builder));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::ExternalReference>> source_reference;
    for (const auto& item : proto.source_reference())
        source_reference.push_back(ConvertExternalReference(item, builder));
    b.add_source_reference(builder.CreateVector(source_reference));
    
    return b.Finish();
}

// ==========================
// LaneBoundary - BoundaryPoint
// ==========================

::flatbuffers::Offset<fbs::osi3::LaneBoundary_::BoundaryPoint> ConvertLaneBoundaryBoundaryPoint(const osi3::LaneBoundary::BoundaryPoint& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::LaneBoundary_::BoundaryPointBuilder b(builder);
    
    if (proto.has_position())
    	b.add_position(ConvertVector3d(proto.position(), builder));
    
    if (proto.has_width())
    	b.add_width(proto.width());
    
    if (proto.has_height())
    	b.add_height(proto.height());
    
    if (proto.has_dash())
    	b.add_dash(static_cast<fbs::osi3::LaneBoundary_::BoundaryPoint_::Dash>(proto.dash()));
    
    return b.Finish();
}

// ==========================
// LaneBoundary - Classification
// ==========================

::flatbuffers::Offset<fbs::osi3::LaneBoundary_::Classification> ConvertLaneBoundaryClassification(const osi3::LaneBoundary::Classification& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::LaneBoundary_::ClassificationBuilder b(builder);
    
    if (proto.has_type())
    	b.add_type(static_cast<fbs::osi3::LaneBoundary_::Classification_::Type>(proto.type()));
    
    if (proto.has_color())
    	b.add_color(static_cast<fbs::osi3::LaneBoundary_::Classification_::Color>(proto.color()));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Identifier>> limiting_structure_id;
    for (const auto& item : proto.limiting_structure_id())
        limiting_structure_id.push_back(ConvertIdentifier(item, builder));
    b.add_limiting_structure_id(builder.CreateVector(limiting_structure_id));
    
    return b.Finish();
}

// ==========================
// LaneBoundary
// ==========================

::flatbuffers::Offset<fbs::osi3::LaneBoundary> ConvertLaneBoundary(const osi3::LaneBoundary& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::LaneBoundaryBuilder b(builder);
    
    if (proto.has_id())
    	b.add_id(ConvertIdentifier(proto.id(), builder));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::LaneBoundary_::BoundaryPoint>> boundary_line;
    for (const auto& item : proto.boundary_line())
        boundary_line.push_back(ConvertLaneBoundaryBoundaryPoint(item, builder));
    b.add_boundary_line(builder.CreateVector(boundary_line));
    
    if (proto.has_classification())
    	b.add_classification(ConvertLaneBoundaryClassification(proto.classification(), builder));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::ExternalReference>> source_reference;
    for (const auto& item : proto.source_reference())
        source_reference.push_back(ConvertExternalReference(item, builder));
    b.add_source_reference(builder.CreateVector(source_reference));
    
    if (proto.has_color_description())
    	b.add_color_description(ConvertColorDescription(proto.color_description(), builder));
    
    return b.Finish();
}
