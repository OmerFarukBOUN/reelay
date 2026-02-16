#include <flatbuffers/flatbuffers.h>
namespace fbs
{
#include "osi_logicallane_generated.h"
}

#include "osi_logicallane.pb.h"

#include <vector>

#include "osi_common_converter.hpp"
#include "osi_object_converter.hpp"
#include "osi_trafficsign_converter.hpp"

// ==========================
// LogicalLaneBoundary - LogicalBoundaryPoint
// ==========================

::flatbuffers::Offset<fbs::osi3::LogicalLaneBoundary_::LogicalBoundaryPoint> ConvertLogicalLaneBoundaryLogicalBoundaryPoint(const osi3::LogicalLaneBoundary::LogicalBoundaryPoint& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::LogicalLaneBoundary_::LogicalBoundaryPointBuilder b(builder);
    
    if (proto.has_position())
    	b.add_position(ConvertVector3d(proto.position(), builder));
    
    if (proto.has_s_position())
    	b.add_s_position(proto.s_position());
    
    if (proto.has_t_position())
    	b.add_t_position(proto.t_position());
    
    return b.Finish();
}

// ==========================
// LogicalLaneBoundary
// ==========================

::flatbuffers::Offset<fbs::osi3::LogicalLaneBoundary> ConvertLogicalLaneBoundary(const osi3::LogicalLaneBoundary& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::LogicalLaneBoundaryBuilder b(builder);
    
    if (proto.has_id())
    	b.add_id(ConvertIdentifier(proto.id(), builder));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::LogicalLaneBoundary_::LogicalBoundaryPoint>> boundary_line;
    for (const auto& item : proto.boundary_line())
        boundary_line.push_back(ConvertLogicalLaneBoundaryLogicalBoundaryPoint(item, builder));
    b.add_boundary_line(builder.CreateVector(boundary_line));
    
    if (proto.has_reference_line_id())
    	b.add_reference_line_id(ConvertIdentifier(proto.reference_line_id(), builder));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Identifier>> physical_boundary_id;
    for (const auto& item : proto.physical_boundary_id())
        physical_boundary_id.push_back(ConvertIdentifier(item, builder));
    b.add_physical_boundary_id(builder.CreateVector(physical_boundary_id));
    
    if (proto.has_passing_rule())
    	b.add_passing_rule(static_cast<fbs::osi3::LogicalLaneBoundary_::PassingRule>(proto.passing_rule()));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::ExternalReference>> source_reference;
    for (const auto& item : proto.source_reference())
        source_reference.push_back(ConvertExternalReference(item, builder));
    b.add_source_reference(builder.CreateVector(source_reference));
    
    return b.Finish();
}

// ==========================
// LogicalLane - PhysicalLaneReference
// ==========================

::flatbuffers::Offset<fbs::osi3::LogicalLane_::PhysicalLaneReference> ConvertLogicalLanePhysicalLaneReference(const osi3::LogicalLane::PhysicalLaneReference& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::LogicalLane_::PhysicalLaneReferenceBuilder b(builder);
    
    if (proto.has_physical_lane_id())
    	b.add_physical_lane_id(ConvertIdentifier(proto.physical_lane_id(), builder));
    
    if (proto.has_start_s())
    	b.add_start_s(proto.start_s());
    
    if (proto.has_end_s())
    	b.add_end_s(proto.end_s());
    
    return b.Finish();
}

// ==========================
// LogicalLane - LaneConnection
// ==========================

::flatbuffers::Offset<fbs::osi3::LogicalLane_::LaneConnection> ConvertLogicalLaneLaneConnection(const osi3::LogicalLane::LaneConnection& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::LogicalLane_::LaneConnectionBuilder b(builder);
    
    if (proto.has_other_lane_id())
    	b.add_other_lane_id(ConvertIdentifier(proto.other_lane_id(), builder));
    
    if (proto.has_at_begin_of_other_lane())
    	b.add_at_begin_of_other_lane(proto.at_begin_of_other_lane());
    
    return b.Finish();
}

// ==========================
// LogicalLane - LaneRelation
// ==========================

::flatbuffers::Offset<fbs::osi3::LogicalLane_::LaneRelation> ConvertLogicalLaneLaneRelation(const osi3::LogicalLane::LaneRelation& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::LogicalLane_::LaneRelationBuilder b(builder);
    
    if (proto.has_other_lane_id())
    	b.add_other_lane_id(ConvertIdentifier(proto.other_lane_id(), builder));
    
    if (proto.has_start_s())
    	b.add_start_s(proto.start_s());
    
    if (proto.has_end_s())
    	b.add_end_s(proto.end_s());
    
    if (proto.has_start_s_other())
    	b.add_start_s_other(proto.start_s_other());
    
    if (proto.has_end_s_other())
    	b.add_end_s_other(proto.end_s_other());
    
    return b.Finish();
}

// ==========================
// LogicalLane - TrafficRule - TrafficRuleValidity - TypeValidity
// ==========================

::flatbuffers::Offset<fbs::osi3::LogicalLane_::TrafficRule_::TrafficRuleValidity_::TypeValidity> ConvertLogicalLaneTrafficRuleTrafficRuleValidityTypeValidity(const osi3::LogicalLane::TrafficRule::TrafficRuleValidity::TypeValidity& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::LogicalLane_::TrafficRule_::TrafficRuleValidity_::TypeValidityBuilder b(builder);
    
    if (proto.has_type())
    	b.add_type(static_cast<fbs::osi3::MovingObject_::Type>(proto.type()));
    
    if (proto.has_vehicle_type())
    	b.add_vehicle_type(static_cast<fbs::osi3::MovingObject_::VehicleClassification_::Type>(proto.vehicle_type()));
    
    if (proto.has_vehicle_role())
    	b.add_vehicle_role(static_cast<fbs::osi3::MovingObject_::VehicleClassification_::Role>(proto.vehicle_role()));
    
    return b.Finish();
}

// ==========================
// LogicalLane - TrafficRule - TrafficRuleValidity
// ==========================

::flatbuffers::Offset<fbs::osi3::LogicalLane_::TrafficRule_::TrafficRuleValidity> ConvertLogicalLaneTrafficRuleTrafficRuleValidity(const osi3::LogicalLane::TrafficRule::TrafficRuleValidity& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::LogicalLane_::TrafficRule_::TrafficRuleValidityBuilder b(builder);
    
    if (proto.has_start_s())
    	b.add_start_s(proto.start_s());
    
    if (proto.has_end_s())
    	b.add_end_s(proto.end_s());
    
    std::vector<::flatbuffers::Offset<fbs::osi3::LogicalLane_::TrafficRule_::TrafficRuleValidity_::TypeValidity>> valid_for_type;
    for (const auto& item : proto.valid_for_type())
        valid_for_type.push_back(ConvertLogicalLaneTrafficRuleTrafficRuleValidityTypeValidity(item, builder));
    b.add_valid_for_type(builder.CreateVector(valid_for_type));
    
    return b.Finish();
}

// ==========================
// LogicalLane - TrafficRule - SpeedLimit
// ==========================

::flatbuffers::Offset<fbs::osi3::LogicalLane_::TrafficRule_::SpeedLimit> ConvertLogicalLaneTrafficRuleSpeedLimit(const osi3::LogicalLane::TrafficRule::SpeedLimit& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::LogicalLane_::TrafficRule_::SpeedLimitBuilder b(builder);
    
    if (proto.has_speed_limit_value())
    	b.add_speed_limit_value(ConvertTrafficSignValue(proto.speed_limit_value(), builder));
    
    return b.Finish();
}

// ==========================
// LogicalLane - TrafficRule
// ==========================

::flatbuffers::Offset<fbs::osi3::LogicalLane_::TrafficRule> ConvertLogicalLaneTrafficRule(const osi3::LogicalLane::TrafficRule& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::LogicalLane_::TrafficRuleBuilder b(builder);
    
    if (proto.has_traffic_rule_type())
    	b.add_traffic_rule_type(static_cast<fbs::osi3::LogicalLane_::TrafficRule_::TrafficRuleType>(proto.traffic_rule_type()));
    
    if (proto.has_traffic_rule_validity())
    	b.add_traffic_rule_validity(ConvertLogicalLaneTrafficRuleTrafficRuleValidity(proto.traffic_rule_validity(), builder));
    
    if (proto.has_speed_limit())
    	b.add_speed_limit(ConvertLogicalLaneTrafficRuleSpeedLimit(proto.speed_limit(), builder));
    
    return b.Finish();
}

// ==========================
// LogicalLane
// ==========================

::flatbuffers::Offset<fbs::osi3::LogicalLane> ConvertLogicalLane(const osi3::LogicalLane& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::LogicalLaneBuilder b(builder);
    
    if (proto.has_id())
    	b.add_id(ConvertIdentifier(proto.id(), builder));
    
    if (proto.has_type())
    	b.add_type(static_cast<fbs::osi3::LogicalLane_::Type>(proto.type()));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::ExternalReference>> source_reference;
    for (const auto& item : proto.source_reference())
        source_reference.push_back(ConvertExternalReference(item, builder));
    b.add_source_reference(builder.CreateVector(source_reference));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::LogicalLane_::PhysicalLaneReference>> physical_lane_reference;
    for (const auto& item : proto.physical_lane_reference())
        physical_lane_reference.push_back(ConvertLogicalLanePhysicalLaneReference(item, builder));
    b.add_physical_lane_reference(builder.CreateVector(physical_lane_reference));
    
    if (proto.has_reference_line_id())
    	b.add_reference_line_id(ConvertIdentifier(proto.reference_line_id(), builder));
    
    if (proto.has_start_s())
    	b.add_start_s(proto.start_s());
    
    if (proto.has_end_s())
    	b.add_end_s(proto.end_s());
    
    if (proto.has_move_direction())
    	b.add_move_direction(static_cast<fbs::osi3::LogicalLane_::MoveDirection>(proto.move_direction()));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::LogicalLane_::LaneRelation>> right_adjacent_lane;
    for (const auto& item : proto.right_adjacent_lane())
        right_adjacent_lane.push_back(ConvertLogicalLaneLaneRelation(item, builder));
    b.add_right_adjacent_lane(builder.CreateVector(right_adjacent_lane));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::LogicalLane_::LaneRelation>> left_adjacent_lane;
    for (const auto& item : proto.left_adjacent_lane())
        left_adjacent_lane.push_back(ConvertLogicalLaneLaneRelation(item, builder));
    b.add_left_adjacent_lane(builder.CreateVector(left_adjacent_lane));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::LogicalLane_::LaneRelation>> overlapping_lane;
    for (const auto& item : proto.overlapping_lane())
        overlapping_lane.push_back(ConvertLogicalLaneLaneRelation(item, builder));
    b.add_overlapping_lane(builder.CreateVector(overlapping_lane));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Identifier>> right_boundary_id;
    for (const auto& item : proto.right_boundary_id())
        right_boundary_id.push_back(ConvertIdentifier(item, builder));
    b.add_right_boundary_id(builder.CreateVector(right_boundary_id));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Identifier>> left_boundary_id;
    for (const auto& item : proto.left_boundary_id())
        left_boundary_id.push_back(ConvertIdentifier(item, builder));
    b.add_left_boundary_id(builder.CreateVector(left_boundary_id));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::LogicalLane_::LaneConnection>> predecessor_lane;
    for (const auto& item : proto.predecessor_lane())
        predecessor_lane.push_back(ConvertLogicalLaneLaneConnection(item, builder));
    b.add_predecessor_lane(builder.CreateVector(predecessor_lane));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::LogicalLane_::LaneConnection>> successor_lane;
    for (const auto& item : proto.successor_lane())
        successor_lane.push_back(ConvertLogicalLaneLaneConnection(item, builder));
    b.add_successor_lane(builder.CreateVector(successor_lane));
    
    if (proto.has_street_name())
    	b.add_street_name(builder.CreateString(proto.street_name()));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::LogicalLane_::TrafficRule>> traffic_rule;
    for (const auto& item : proto.traffic_rule())
        traffic_rule.push_back(ConvertLogicalLaneTrafficRule(item, builder));
    b.add_traffic_rule(builder.CreateVector(traffic_rule));
    
    return b.Finish();
}
