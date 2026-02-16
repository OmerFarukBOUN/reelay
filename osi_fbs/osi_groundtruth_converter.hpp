#include <flatbuffers/flatbuffers.h>
namespace fbs
{
#include "osi_groundtruth_generated.h"
}

#include "osi_groundtruth.pb.h"

#include <vector>

#include "osi_common_converter.hpp"
#include "osi_environment_converter.hpp"
#include "osi_trafficsign_converter.hpp"
#include "osi_trafficlight_converter.hpp"
#include "osi_roadmarking_converter.hpp"
#include "osi_lane_converter.hpp"
#include "osi_logicallane_converter.hpp"
#include "osi_referenceline_converter.hpp"
#include "osi_object_converter.hpp"
#include "osi_occupant_converter.hpp"

// ==========================
// GroundTruth - ProjFrameOffset
// ==========================

::flatbuffers::Offset<fbs::osi3::GroundTruth_::ProjFrameOffset> ConvertGroundTruthProjFrameOffset(const osi3::GroundTruth::ProjFrameOffset& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::GroundTruth_::ProjFrameOffsetBuilder b(builder);
    
    if (proto.has_position())
    	b.add_position(ConvertVector3d(proto.position(), builder));
    
    if (proto.has_yaw())
    	b.add_yaw(proto.yaw());
    
    return b.Finish();
}

// ==========================
// GroundTruth
// ==========================

::flatbuffers::Offset<fbs::osi3::GroundTruth> ConvertGroundTruth(const osi3::GroundTruth& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::GroundTruthBuilder b(builder);
    
    if (proto.has_timestamp())
    	b.add_timestamp(ConvertTimestamp(proto.timestamp(), builder));
    
    if (proto.has_host_vehicle_id())
    	b.add_host_vehicle_id(ConvertIdentifier(proto.host_vehicle_id(), builder));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::StationaryObject>> stationary_object;
    for (const auto& item : proto.stationary_object())
        stationary_object.push_back(ConvertStationaryObject(item, builder));
    b.add_stationary_object(builder.CreateVector(stationary_object));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::MovingObject>> moving_object;
    for (const auto& item : proto.moving_object())
        moving_object.push_back(ConvertMovingObject(item, builder));
    b.add_moving_object(builder.CreateVector(moving_object));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::TrafficSign>> traffic_sign;
    for (const auto& item : proto.traffic_sign())
        traffic_sign.push_back(ConvertTrafficSign(item, builder));
    b.add_traffic_sign(builder.CreateVector(traffic_sign));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::TrafficLight>> traffic_light;
    for (const auto& item : proto.traffic_light())
        traffic_light.push_back(ConvertTrafficLight(item, builder));
    b.add_traffic_light(builder.CreateVector(traffic_light));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::RoadMarking>> road_marking;
    for (const auto& item : proto.road_marking())
        road_marking.push_back(ConvertRoadMarking(item, builder));
    b.add_road_marking(builder.CreateVector(road_marking));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::LaneBoundary>> lane_boundary;
    for (const auto& item : proto.lane_boundary())
        lane_boundary.push_back(ConvertLaneBoundary(item, builder));
    b.add_lane_boundary(builder.CreateVector(lane_boundary));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Lane>> lane;
    for (const auto& item : proto.lane())
        lane.push_back(ConvertLane(item, builder));
    b.add_lane(builder.CreateVector(lane));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Occupant>> occupant;
    for (const auto& item : proto.occupant())
        occupant.push_back(ConvertOccupant(item, builder));
    b.add_occupant(builder.CreateVector(occupant));
    
    if (proto.has_environmental_conditions())
    	b.add_environmental_conditions(ConvertEnvironmentalConditions(proto.environmental_conditions(), builder));
    
    if (proto.has_country_code())
    	b.add_country_code(proto.country_code());
    
    if (proto.has_proj_string())
    	b.add_proj_string(builder.CreateString(proto.proj_string()));
    
    if (proto.has_map_reference())
    	b.add_map_reference(builder.CreateString(proto.map_reference()));
    
    if (proto.has_model_reference())
    	b.add_model_reference(builder.CreateString(proto.model_reference()));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::ReferenceLine>> reference_line;
    for (const auto& item : proto.reference_line())
        reference_line.push_back(ConvertReferenceLine(item, builder));
    b.add_reference_line(builder.CreateVector(reference_line));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::LogicalLaneBoundary>> logical_lane_boundary;
    for (const auto& item : proto.logical_lane_boundary())
        logical_lane_boundary.push_back(ConvertLogicalLaneBoundary(item, builder));
    b.add_logical_lane_boundary(builder.CreateVector(logical_lane_boundary));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::LogicalLane>> logical_lane;
    for (const auto& item : proto.logical_lane())
        logical_lane.push_back(ConvertLogicalLane(item, builder));
    b.add_logical_lane(builder.CreateVector(logical_lane));
    
    if (proto.has_proj_frame_offset())
    	b.add_proj_frame_offset(ConvertGroundTruthProjFrameOffset(proto.proj_frame_offset(), builder));
    
    return b.Finish();
}
