#include <flatbuffers/flatbuffers.h>
namespace fbs
{
#include "osi_object_generated.h"
}

#include "osi_object.pb.h"

#include <vector>

#include "osi_common_converter.hpp"

// ==========================
// StationaryObject - Classification - EmittingStructureAttribute
// ==========================

::flatbuffers::Offset<fbs::osi3::StationaryObject_::Classification_::EmittingStructureAttribute> ConvertStationaryObjectClassificationEmittingStructureAttribute(const osi3::StationaryObject::Classification::EmittingStructureAttribute& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::StationaryObject_::Classification_::EmittingStructureAttributeBuilder b(builder);
    
    std::vector<::flatbuffers::Offset<fbs::osi3::WavelengthData>> wavelength_data;
    for (const auto& item : proto.wavelength_data())
        wavelength_data.push_back(ConvertWavelengthData(item, builder));
    b.add_wavelength_data(builder.CreateVector(wavelength_data));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::SpatialSignalStrength>> emitted_spatial_signal_strength;
    for (const auto& item : proto.emitted_spatial_signal_strength())
        emitted_spatial_signal_strength.push_back(ConvertSpatialSignalStrength(item, builder));
    b.add_emitted_spatial_signal_strength(builder.CreateVector(emitted_spatial_signal_strength));
    
    return b.Finish();
}

// ==========================
// StationaryObject - Classification
// ==========================

::flatbuffers::Offset<fbs::osi3::StationaryObject_::Classification> ConvertStationaryObjectClassification(const osi3::StationaryObject::Classification& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::StationaryObject_::ClassificationBuilder b(builder);
    
    if (proto.has_type())
    	b.add_type(static_cast<fbs::osi3::StationaryObject_::Classification_::Type>(proto.type()));
    
    if (proto.has_material())
    	b.add_material(static_cast<fbs::osi3::StationaryObject_::Classification_::Material>(proto.material()));
    
    if (proto.has_density())
    	b.add_density(static_cast<fbs::osi3::StationaryObject_::Classification_::Density>(proto.density()));
    
    if (proto.has_color())
    	b.add_color(static_cast<fbs::osi3::StationaryObject_::Classification_::Color>(proto.color()));
    
    if (proto.has_emitting_structure_attribute())
    	b.add_emitting_structure_attribute(ConvertStationaryObjectClassificationEmittingStructureAttribute(proto.emitting_structure_attribute(), builder));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Identifier>> assigned_lane_id;
    for (const auto& item : proto.assigned_lane_id())
        assigned_lane_id.push_back(ConvertIdentifier(item, builder));
    b.add_assigned_lane_id(builder.CreateVector(assigned_lane_id));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::double>> assigned_lane_percentage;
    for (const auto& item : proto.assigned_lane_percentage())
        assigned_lane_percentage.push_back(Convertdouble(item, builder));
    b.add_assigned_lane_percentage(builder.CreateVector(assigned_lane_percentage));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::LogicalLaneAssignment>> logical_lane_assignment;
    for (const auto& item : proto.logical_lane_assignment())
        logical_lane_assignment.push_back(ConvertLogicalLaneAssignment(item, builder));
    b.add_logical_lane_assignment(builder.CreateVector(logical_lane_assignment));
    
    return b.Finish();
}

// ==========================
// StationaryObject
// ==========================

::flatbuffers::Offset<fbs::osi3::StationaryObject> ConvertStationaryObject(const osi3::StationaryObject& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::StationaryObjectBuilder b(builder);
    
    if (proto.has_id())
    	b.add_id(ConvertIdentifier(proto.id(), builder));
    
    if (proto.has_base())
    	b.add_base(ConvertBaseStationary(proto.base(), builder));
    
    if (proto.has_classification())
    	b.add_classification(ConvertStationaryObjectClassification(proto.classification(), builder));
    
    if (proto.has_model_reference())
    	b.add_model_reference(builder.CreateString(proto.model_reference()));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::ExternalReference>> source_reference;
    for (const auto& item : proto.source_reference())
        source_reference.push_back(ConvertExternalReference(item, builder));
    b.add_source_reference(builder.CreateVector(source_reference));
    
    if (proto.has_color_description())
    	b.add_color_description(ConvertColorDescription(proto.color_description(), builder));
    
    return b.Finish();
}

// ==========================
// MovingObject - VehicleAttributes - WheelData
// ==========================

::flatbuffers::Offset<fbs::osi3::MovingObject_::VehicleAttributes_::WheelData> ConvertMovingObjectVehicleAttributesWheelData(const osi3::MovingObject::VehicleAttributes::WheelData& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::MovingObject_::VehicleAttributes_::WheelDataBuilder b(builder);
    
    if (proto.has_axle())
    	b.add_axle(proto.axle());
    
    if (proto.has_index())
    	b.add_index(proto.index());
    
    if (proto.has_position())
    	b.add_position(ConvertVector3d(proto.position(), builder));
    
    if (proto.has_wheel_radius())
    	b.add_wheel_radius(proto.wheel_radius());
    
    if (proto.has_rim_radius())
    	b.add_rim_radius(proto.rim_radius());
    
    if (proto.has_width())
    	b.add_width(proto.width());
    
    if (proto.has_orientation())
    	b.add_orientation(ConvertOrientation3d(proto.orientation(), builder));
    
    if (proto.has_rotation_rate())
    	b.add_rotation_rate(proto.rotation_rate());
    
    if (proto.has_model_reference())
    	b.add_model_reference(builder.CreateString(proto.model_reference()));
    
    if (proto.has_friction_coefficient())
    	b.add_friction_coefficient(proto.friction_coefficient());
    
    return b.Finish();
}

// ==========================
// MovingObject - VehicleAttributes
// ==========================

::flatbuffers::Offset<fbs::osi3::MovingObject_::VehicleAttributes> ConvertMovingObjectVehicleAttributes(const osi3::MovingObject::VehicleAttributes& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::MovingObject_::VehicleAttributesBuilder b(builder);
    
    if (proto.has_driver_id())
    	b.add_driver_id(ConvertIdentifier(proto.driver_id(), builder));
    
    if (proto.has_radius_wheel())
    	b.add_radius_wheel(proto.radius_wheel());
    
    if (proto.has_number_wheels())
    	b.add_number_wheels(proto.number_wheels());
    
    if (proto.has_bbcenter_to_rear())
    	b.add_bbcenter_to_rear(ConvertVector3d(proto.bbcenter_to_rear(), builder));
    
    if (proto.has_bbcenter_to_front())
    	b.add_bbcenter_to_front(ConvertVector3d(proto.bbcenter_to_front(), builder));
    
    if (proto.has_ground_clearance())
    	b.add_ground_clearance(proto.ground_clearance());
    
    std::vector<::flatbuffers::Offset<fbs::osi3::MovingObject_::VehicleAttributes_::WheelData>> wheel_data;
    for (const auto& item : proto.wheel_data())
        wheel_data.push_back(ConvertMovingObjectVehicleAttributesWheelData(item, builder));
    b.add_wheel_data(builder.CreateVector(wheel_data));
    
    if (proto.has_steering_wheel_angle())
    	b.add_steering_wheel_angle(proto.steering_wheel_angle());
    
    return b.Finish();
}

// ==========================
// MovingObject - MovingObjectClassification
// ==========================

::flatbuffers::Offset<fbs::osi3::MovingObject_::MovingObjectClassification> ConvertMovingObjectMovingObjectClassification(const osi3::MovingObject::MovingObjectClassification& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::MovingObject_::MovingObjectClassificationBuilder b(builder);
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Identifier>> assigned_lane_id;
    for (const auto& item : proto.assigned_lane_id())
        assigned_lane_id.push_back(ConvertIdentifier(item, builder));
    b.add_assigned_lane_id(builder.CreateVector(assigned_lane_id));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::double>> assigned_lane_percentage;
    for (const auto& item : proto.assigned_lane_percentage())
        assigned_lane_percentage.push_back(Convertdouble(item, builder));
    b.add_assigned_lane_percentage(builder.CreateVector(assigned_lane_percentage));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::LogicalLaneAssignment>> logical_lane_assignment;
    for (const auto& item : proto.logical_lane_assignment())
        logical_lane_assignment.push_back(ConvertLogicalLaneAssignment(item, builder));
    b.add_logical_lane_assignment(builder.CreateVector(logical_lane_assignment));
    
    return b.Finish();
}

// ==========================
// MovingObject - VehicleClassification - LightState
// ==========================

::flatbuffers::Offset<fbs::osi3::MovingObject_::VehicleClassification_::LightState> ConvertMovingObjectVehicleClassificationLightState(const osi3::MovingObject::VehicleClassification::LightState& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::MovingObject_::VehicleClassification_::LightStateBuilder b(builder);
    
    if (proto.has_indicator_state())
    	b.add_indicator_state(static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::IndicatorState>(proto.indicator_state()));
    
    if (proto.has_front_fog_light())
    	b.add_front_fog_light(static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(proto.front_fog_light()));
    
    if (proto.has_rear_fog_light())
    	b.add_rear_fog_light(static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(proto.rear_fog_light()));
    
    if (proto.has_head_light())
    	b.add_head_light(static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(proto.head_light()));
    
    if (proto.has_high_beam())
    	b.add_high_beam(static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(proto.high_beam()));
    
    if (proto.has_reversing_light())
    	b.add_reversing_light(static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(proto.reversing_light()));
    
    if (proto.has_tail_light())
    	b.add_tail_light(static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(proto.tail_light()));
    
    if (proto.has_brake_light_state())
    	b.add_brake_light_state(static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::BrakeLightState>(proto.brake_light_state()));
    
    if (proto.has_license_plate_illumination_rear())
    	b.add_license_plate_illumination_rear(static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(proto.license_plate_illumination_rear()));
    
    if (proto.has_emergency_vehicle_illumination())
    	b.add_emergency_vehicle_illumination(static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(proto.emergency_vehicle_illumination()));
    
    if (proto.has_service_vehicle_illumination())
    	b.add_service_vehicle_illumination(static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(proto.service_vehicle_illumination()));
    
    return b.Finish();
}

// ==========================
// MovingObject - VehicleClassification
// ==========================

::flatbuffers::Offset<fbs::osi3::MovingObject_::VehicleClassification> ConvertMovingObjectVehicleClassification(const osi3::MovingObject::VehicleClassification& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::MovingObject_::VehicleClassificationBuilder b(builder);
    
    if (proto.has_type())
    	b.add_type(static_cast<fbs::osi3::MovingObject_::VehicleClassification_::Type>(proto.type()));
    
    if (proto.has_light_state())
    	b.add_light_state(ConvertMovingObjectVehicleClassificationLightState(proto.light_state(), builder));
    
    if (proto.has_has_trailer())
    	b.add_has_trailer(proto.has_trailer());
    
    if (proto.has_trailer_id())
    	b.add_trailer_id(ConvertIdentifier(proto.trailer_id(), builder));
    
    if (proto.has_role())
    	b.add_role(static_cast<fbs::osi3::MovingObject_::VehicleClassification_::Role>(proto.role()));
    
    return b.Finish();
}

// ==========================
// MovingObject - PedestrianAttributes - Bone
// ==========================

::flatbuffers::Offset<fbs::osi3::MovingObject_::PedestrianAttributes_::Bone> ConvertMovingObjectPedestrianAttributesBone(const osi3::MovingObject::PedestrianAttributes::Bone& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::MovingObject_::PedestrianAttributes_::BoneBuilder b(builder);
    
    if (proto.has_type())
    	b.add_type(static_cast<fbs::osi3::MovingObject_::PedestrianAttributes_::Bone_::Type>(proto.type()));
    
    if (proto.has_position())
    	b.add_position(ConvertVector3d(proto.position(), builder));
    
    if (proto.has_orientation())
    	b.add_orientation(ConvertOrientation3d(proto.orientation(), builder));
    
    if (proto.has_length())
    	b.add_length(proto.length());
    
    if (proto.has_missing())
    	b.add_missing(proto.missing());
    
    if (proto.has_velocity())
    	b.add_velocity(ConvertVector3d(proto.velocity(), builder));
    
    if (proto.has_orientation_rate())
    	b.add_orientation_rate(ConvertOrientation3d(proto.orientation_rate(), builder));
    
    return b.Finish();
}

// ==========================
// MovingObject - PedestrianAttributes
// ==========================

::flatbuffers::Offset<fbs::osi3::MovingObject_::PedestrianAttributes> ConvertMovingObjectPedestrianAttributes(const osi3::MovingObject::PedestrianAttributes& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::MovingObject_::PedestrianAttributesBuilder b(builder);
    
    if (proto.has_bbcenter_to_root())
    	b.add_bbcenter_to_root(ConvertVector3d(proto.bbcenter_to_root(), builder));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::MovingObject_::PedestrianAttributes_::Bone>> skeleton_bone;
    for (const auto& item : proto.skeleton_bone())
        skeleton_bone.push_back(ConvertMovingObjectPedestrianAttributesBone(item, builder));
    b.add_skeleton_bone(builder.CreateVector(skeleton_bone));
    
    return b.Finish();
}

// ==========================
// MovingObject
// ==========================

::flatbuffers::Offset<fbs::osi3::MovingObject> ConvertMovingObject(const osi3::MovingObject& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::MovingObjectBuilder b(builder);
    
    if (proto.has_id())
    	b.add_id(ConvertIdentifier(proto.id(), builder));
    
    if (proto.has_base())
    	b.add_base(ConvertBaseMoving(proto.base(), builder));
    
    if (proto.has_type())
    	b.add_type(static_cast<fbs::osi3::MovingObject_::Type>(proto.type()));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Identifier>> assigned_lane_id;
    for (const auto& item : proto.assigned_lane_id())
        assigned_lane_id.push_back(ConvertIdentifier(item, builder));
    b.add_assigned_lane_id(builder.CreateVector(assigned_lane_id));
    
    if (proto.has_vehicle_attributes())
    	b.add_vehicle_attributes(ConvertMovingObjectVehicleAttributes(proto.vehicle_attributes(), builder));
    
    if (proto.has_vehicle_classification())
    	b.add_vehicle_classification(ConvertMovingObjectVehicleClassification(proto.vehicle_classification(), builder));
    
    if (proto.has_model_reference())
    	b.add_model_reference(builder.CreateString(proto.model_reference()));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::StatePoint>> future_trajectory;
    for (const auto& item : proto.future_trajectory())
        future_trajectory.push_back(ConvertStatePoint(item, builder));
    b.add_future_trajectory(builder.CreateVector(future_trajectory));
    
    if (proto.has_moving_object_classification())
    	b.add_moving_object_classification(ConvertMovingObjectMovingObjectClassification(proto.moving_object_classification(), builder));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::ExternalReference>> source_reference;
    for (const auto& item : proto.source_reference())
        source_reference.push_back(ConvertExternalReference(item, builder));
    b.add_source_reference(builder.CreateVector(source_reference));
    
    if (proto.has_color_description())
    	b.add_color_description(ConvertColorDescription(proto.color_description(), builder));
    
    if (proto.has_pedestrian_attributes())
    	b.add_pedestrian_attributes(ConvertMovingObjectPedestrianAttributes(proto.pedestrian_attributes(), builder));
    
    return b.Finish();
}
