#include <flatbuffers/flatbuffers.h>
namespace fbs
{
#include "osi_common_generated.h"
}

#include "osi_common.pb.h"

#include <vector>

// ==========================
// Vector3d
// ==========================

::flatbuffers::Offset<fbs::osi3::Vector3d> ConvertVector3d(const osi3::Vector3d& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::Vector3dBuilder b(builder);
    
    if (proto.has_x())
    	b.add_x(proto.x());
    
    if (proto.has_y())
    	b.add_y(proto.y());
    
    if (proto.has_z())
    	b.add_z(proto.z());
    
    return b.Finish();
}

// ==========================
// Vector2d
// ==========================

::flatbuffers::Offset<fbs::osi3::Vector2d> ConvertVector2d(const osi3::Vector2d& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::Vector2dBuilder b(builder);
    
    if (proto.has_x())
    	b.add_x(proto.x());
    
    if (proto.has_y())
    	b.add_y(proto.y());
    
    return b.Finish();
}

// ==========================
// Timestamp
// ==========================

::flatbuffers::Offset<fbs::osi3::Timestamp> ConvertTimestamp(const osi3::Timestamp& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::TimestampBuilder b(builder);
    
    if (proto.has_seconds())
    	b.add_seconds(proto.seconds());
    
    if (proto.has_nanos())
    	b.add_nanos(proto.nanos());
    
    return b.Finish();
}

// ==========================
// Dimension3d
// ==========================

::flatbuffers::Offset<fbs::osi3::Dimension3d> ConvertDimension3d(const osi3::Dimension3d& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::Dimension3dBuilder b(builder);
    
    if (proto.has_length())
    	b.add_length(proto.length());
    
    if (proto.has_width())
    	b.add_width(proto.width());
    
    if (proto.has_height())
    	b.add_height(proto.height());
    
    return b.Finish();
}

// ==========================
// Orientation3d
// ==========================

::flatbuffers::Offset<fbs::osi3::Orientation3d> ConvertOrientation3d(const osi3::Orientation3d& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::Orientation3dBuilder b(builder);
    
    if (proto.has_roll())
    	b.add_roll(proto.roll());
    
    if (proto.has_pitch())
    	b.add_pitch(proto.pitch());
    
    if (proto.has_yaw())
    	b.add_yaw(proto.yaw());
    
    return b.Finish();
}

// ==========================
// Identifier
// ==========================

::flatbuffers::Offset<fbs::osi3::Identifier> ConvertIdentifier(const osi3::Identifier& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::IdentifierBuilder b(builder);
    
    if (proto.has_value())
    	b.add_value(proto.value());
    
    return b.Finish();
}

// ==========================
// ExternalReference
// ==========================

::flatbuffers::Offset<fbs::osi3::ExternalReference> ConvertExternalReference(const osi3::ExternalReference& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::ExternalReferenceBuilder b(builder);
    
    if (proto.has_reference())
    	b.add_reference(builder.CreateString(proto.reference()));
    
    if (proto.has_type())
    	b.add_type(builder.CreateString(proto.type()));
    
    std::vector<::flatbuffers::Offset<::flatbuffers::String>> identifier;
    for (const auto& item : proto.identifier())
        identifier.push_back(builder.CreateString(item));
    b.add_identifier(builder.CreateVector(identifier));
    
    return b.Finish();
}

// ==========================
// MountingPosition
// ==========================

::flatbuffers::Offset<fbs::osi3::MountingPosition> ConvertMountingPosition(const osi3::MountingPosition& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::MountingPositionBuilder b(builder);
    
    if (proto.has_position())
    	b.add_position(ConvertVector3d(proto.position(), builder));
    
    if (proto.has_orientation())
    	b.add_orientation(ConvertOrientation3d(proto.orientation(), builder));
    
    return b.Finish();
}

// ==========================
// Spherical3d
// ==========================

::flatbuffers::Offset<fbs::osi3::Spherical3d> ConvertSpherical3d(const osi3::Spherical3d& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::Spherical3dBuilder b(builder);
    
    if (proto.has_distance())
    	b.add_distance(proto.distance());
    
    if (proto.has_azimuth())
    	b.add_azimuth(proto.azimuth());
    
    if (proto.has_elevation())
    	b.add_elevation(proto.elevation());
    
    return b.Finish();
}

// ==========================
// LogicalLaneAssignment
// ==========================

::flatbuffers::Offset<fbs::osi3::LogicalLaneAssignment> ConvertLogicalLaneAssignment(const osi3::LogicalLaneAssignment& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::LogicalLaneAssignmentBuilder b(builder);
    
    if (proto.has_assigned_lane_id())
    	b.add_assigned_lane_id(ConvertIdentifier(proto.assigned_lane_id(), builder));
    
    if (proto.has_s_position())
    	b.add_s_position(proto.s_position());
    
    if (proto.has_t_position())
    	b.add_t_position(proto.t_position());
    
    if (proto.has_angle_to_lane())
    	b.add_angle_to_lane(proto.angle_to_lane());
    
    return b.Finish();
}

// ==========================
// BoundingBox
// ==========================

::flatbuffers::Offset<fbs::osi3::BoundingBox> ConvertBoundingBox(const osi3::BoundingBox& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::BoundingBoxBuilder b(builder);
    
    if (proto.has_dimension())
    	b.add_dimension(ConvertDimension3d(proto.dimension(), builder));
    
    if (proto.has_position())
    	b.add_position(ConvertVector3d(proto.position(), builder));
    
    if (proto.has_orientation())
    	b.add_orientation(ConvertOrientation3d(proto.orientation(), builder));
    
    if (proto.has_contained_object_type())
    	b.add_contained_object_type(static_cast<fbs::osi3::BoundingBox_::Type>(proto.contained_object_type()));
    
    if (proto.has_model_reference())
    	b.add_model_reference(builder.CreateString(proto.model_reference()));
    
    return b.Finish();
}

// ==========================
// BaseStationary
// ==========================

::flatbuffers::Offset<fbs::osi3::BaseStationary> ConvertBaseStationary(const osi3::BaseStationary& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::BaseStationaryBuilder b(builder);
    
    if (proto.has_dimension())
    	b.add_dimension(ConvertDimension3d(proto.dimension(), builder));
    
    if (proto.has_position())
    	b.add_position(ConvertVector3d(proto.position(), builder));
    
    if (proto.has_orientation())
    	b.add_orientation(ConvertOrientation3d(proto.orientation(), builder));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Vector2d>> base_polygon;
    for (const auto& item : proto.base_polygon())
        base_polygon.push_back(ConvertVector2d(item, builder));
    b.add_base_polygon(builder.CreateVector(base_polygon));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::BoundingBox>> bounding_box_section;
    for (const auto& item : proto.bounding_box_section())
        bounding_box_section.push_back(ConvertBoundingBox(item, builder));
    b.add_bounding_box_section(builder.CreateVector(bounding_box_section));
    
    return b.Finish();
}

// ==========================
// BaseMoving
// ==========================

::flatbuffers::Offset<fbs::osi3::BaseMoving> ConvertBaseMoving(const osi3::BaseMoving& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::BaseMovingBuilder b(builder);
    
    if (proto.has_dimension())
    	b.add_dimension(ConvertDimension3d(proto.dimension(), builder));
    
    if (proto.has_position())
    	b.add_position(ConvertVector3d(proto.position(), builder));
    
    if (proto.has_orientation())
    	b.add_orientation(ConvertOrientation3d(proto.orientation(), builder));
    
    if (proto.has_velocity())
    	b.add_velocity(ConvertVector3d(proto.velocity(), builder));
    
    if (proto.has_acceleration())
    	b.add_acceleration(ConvertVector3d(proto.acceleration(), builder));
    
    if (proto.has_orientation_rate())
    	b.add_orientation_rate(ConvertOrientation3d(proto.orientation_rate(), builder));
    
    if (proto.has_orientation_acceleration())
    	b.add_orientation_acceleration(ConvertOrientation3d(proto.orientation_acceleration(), builder));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Vector2d>> base_polygon;
    for (const auto& item : proto.base_polygon())
        base_polygon.push_back(ConvertVector2d(item, builder));
    b.add_base_polygon(builder.CreateVector(base_polygon));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::BoundingBox>> bounding_box_section;
    for (const auto& item : proto.bounding_box_section())
        bounding_box_section.push_back(ConvertBoundingBox(item, builder));
    b.add_bounding_box_section(builder.CreateVector(bounding_box_section));
    
    return b.Finish();
}

// ==========================
// StatePoint
// ==========================

::flatbuffers::Offset<fbs::osi3::StatePoint> ConvertStatePoint(const osi3::StatePoint& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::StatePointBuilder b(builder);
    
    if (proto.has_timestamp())
    	b.add_timestamp(ConvertTimestamp(proto.timestamp(), builder));
    
    if (proto.has_position())
    	b.add_position(ConvertVector3d(proto.position(), builder));
    
    if (proto.has_orientation())
    	b.add_orientation(ConvertOrientation3d(proto.orientation(), builder));
    
    return b.Finish();
}

// ==========================
// WavelengthData
// ==========================

::flatbuffers::Offset<fbs::osi3::WavelengthData> ConvertWavelengthData(const osi3::WavelengthData& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::WavelengthDataBuilder b(builder);
    
    if (proto.has_start())
    	b.add_start(proto.start());
    
    if (proto.has_end())
    	b.add_end(proto.end());
    
    if (proto.has_samples_number())
    	b.add_samples_number(proto.samples_number());
    
    return b.Finish();
}

// ==========================
// SpatialSignalStrength
// ==========================

::flatbuffers::Offset<fbs::osi3::SpatialSignalStrength> ConvertSpatialSignalStrength(const osi3::SpatialSignalStrength& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::SpatialSignalStrengthBuilder b(builder);
    
    if (proto.has_horizontal_angle())
    	b.add_horizontal_angle(proto.horizontal_angle());
    
    if (proto.has_vertical_angle())
    	b.add_vertical_angle(proto.vertical_angle());
    
    if (proto.has_signal_strength())
    	b.add_signal_strength(proto.signal_strength());
    
    return b.Finish();
}

// ==========================
// ColorDescription
// ==========================

::flatbuffers::Offset<fbs::osi3::ColorDescription> ConvertColorDescription(const osi3::ColorDescription& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::ColorDescriptionBuilder b(builder);
    
    if (proto.has_grey())
    	b.add_grey(ConvertColorGrey(proto.grey(), builder));
    
    if (proto.has_rgb())
    	b.add_rgb(ConvertColorRGB(proto.rgb(), builder));
    
    if (proto.has_rgbir())
    	b.add_rgbir(ConvertColorRGBIR(proto.rgbir(), builder));
    
    if (proto.has_hsv())
    	b.add_hsv(ConvertColorHSV(proto.hsv(), builder));
    
    if (proto.has_luv())
    	b.add_luv(ConvertColorLUV(proto.luv(), builder));
    
    if (proto.has_cmyk())
    	b.add_cmyk(ConvertColorCMYK(proto.cmyk(), builder));
    
    return b.Finish();
}

// ==========================
// ColorGrey
// ==========================

::flatbuffers::Offset<fbs::osi3::ColorGrey> ConvertColorGrey(const osi3::ColorGrey& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::ColorGreyBuilder b(builder);
    
    if (proto.has_grey())
    	b.add_grey(proto.grey());
    
    return b.Finish();
}

// ==========================
// ColorRGB
// ==========================

::flatbuffers::Offset<fbs::osi3::ColorRGB> ConvertColorRGB(const osi3::ColorRGB& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::ColorRGBBuilder b(builder);
    
    if (proto.has_red())
    	b.add_red(proto.red());
    
    if (proto.has_green())
    	b.add_green(proto.green());
    
    if (proto.has_blue())
    	b.add_blue(proto.blue());
    
    return b.Finish();
}

// ==========================
// ColorRGBIR
// ==========================

::flatbuffers::Offset<fbs::osi3::ColorRGBIR> ConvertColorRGBIR(const osi3::ColorRGBIR& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::ColorRGBIRBuilder b(builder);
    
    if (proto.has_red())
    	b.add_red(proto.red());
    
    if (proto.has_green())
    	b.add_green(proto.green());
    
    if (proto.has_blue())
    	b.add_blue(proto.blue());
    
    if (proto.has_infrared())
    	b.add_infrared(proto.infrared());
    
    return b.Finish();
}

// ==========================
// ColorHSV
// ==========================

::flatbuffers::Offset<fbs::osi3::ColorHSV> ConvertColorHSV(const osi3::ColorHSV& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::ColorHSVBuilder b(builder);
    
    if (proto.has_hue())
    	b.add_hue(proto.hue());
    
    if (proto.has_saturation())
    	b.add_saturation(proto.saturation());
    
    if (proto.has_value())
    	b.add_value(proto.value());
    
    return b.Finish();
}

// ==========================
// ColorLUV
// ==========================

::flatbuffers::Offset<fbs::osi3::ColorLUV> ConvertColorLUV(const osi3::ColorLUV& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::ColorLUVBuilder b(builder);
    
    if (proto.has_luminance())
    	b.add_luminance(proto.luminance());
    
    if (proto.has_u())
    	b.add_u(proto.u());
    
    if (proto.has_v())
    	b.add_v(proto.v());
    
    return b.Finish();
}

// ==========================
// ColorCMYK
// ==========================

::flatbuffers::Offset<fbs::osi3::ColorCMYK> ConvertColorCMYK(const osi3::ColorCMYK& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::ColorCMYKBuilder b(builder);
    
    if (proto.has_cyan())
    	b.add_cyan(proto.cyan());
    
    if (proto.has_magenta())
    	b.add_magenta(proto.magenta());
    
    if (proto.has_yellow())
    	b.add_yellow(proto.yellow());
    
    if (proto.has_key())
    	b.add_key(proto.key());
    
    return b.Finish();
}

// ==========================
// Pedalry
// ==========================

::flatbuffers::Offset<fbs::osi3::Pedalry> ConvertPedalry(const osi3::Pedalry& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::PedalryBuilder b(builder);
    
    if (proto.has_pedal_position_acceleration())
    	b.add_pedal_position_acceleration(proto.pedal_position_acceleration());
    
    if (proto.has_pedal_position_brake())
    	b.add_pedal_position_brake(proto.pedal_position_brake());
    
    if (proto.has_pedal_position_clutch())
    	b.add_pedal_position_clutch(proto.pedal_position_clutch());
    
    return b.Finish();
}

// ==========================
// VehicleSteeringWheel
// ==========================

::flatbuffers::Offset<fbs::osi3::VehicleSteeringWheel> ConvertVehicleSteeringWheel(const osi3::VehicleSteeringWheel& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::VehicleSteeringWheelBuilder b(builder);
    
    if (proto.has_angle())
    	b.add_angle(proto.angle());
    
    if (proto.has_angular_speed())
    	b.add_angular_speed(proto.angular_speed());
    
    if (proto.has_torque())
    	b.add_torque(proto.torque());
    
    return b.Finish();
}

// ==========================
// GeodeticPosition
// ==========================

::flatbuffers::Offset<fbs::osi3::GeodeticPosition> ConvertGeodeticPosition(const osi3::GeodeticPosition& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::GeodeticPositionBuilder b(builder);
    
    if (proto.has_longitude())
    	b.add_longitude(proto.longitude());
    
    if (proto.has_latitude())
    	b.add_latitude(proto.latitude());
    
    if (proto.has_altitude())
    	b.add_altitude(proto.altitude());
    
    return b.Finish();
}

// ==========================
// KeyValuePair
// ==========================

::flatbuffers::Offset<fbs::osi3::KeyValuePair> ConvertKeyValuePair(const osi3::KeyValuePair& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::KeyValuePairBuilder b(builder);
    
    if (proto.has_key())
    	b.add_key(builder.CreateString(proto.key()));
    
    if (proto.has_value())
    	b.add_value(builder.CreateString(proto.value()));
    
    return b.Finish();
}

// ==========================
// Polygon3d
// ==========================

::flatbuffers::Offset<fbs::osi3::Polygon3d> ConvertPolygon3d(const osi3::Polygon3d& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::Polygon3dBuilder b(builder);
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Vector3d>> vertex;
    for (const auto& item : proto.vertex())
        vertex.push_back(ConvertVector3d(item, builder));
    b.add_vertex(builder.CreateVector(vertex));
    
    return b.Finish();
}
