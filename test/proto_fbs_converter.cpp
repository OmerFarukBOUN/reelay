#include <flatbuffers/flatbuffers.h>
namespace fbs
{
#include "osi_groundtruth_generated.h"
}

#include "osi_groundtruth.pb.h"

#include <vector>

// osi_common

// ---------- Basic Types ----------

::flatbuffers::Offset<fbs::osi3::Vector3d> ConvertVector3d(const osi3::Vector3d& v, flatbuffers::FlatBufferBuilder& builder)
{
    auto x = v.has_x() ? v.x() : 0.0;
    auto y = v.has_y() ? v.y() : 0.0;
    auto z = v.has_z() ? v.z() : 0.0;
    return fbs::osi3::CreateVector3d(builder, x, y, z);
}

::flatbuffers::Offset<fbs::osi3::Vector2d> ConvertVector2d(const osi3::Vector2d& v, flatbuffers::FlatBufferBuilder& builder)
{
    auto x = v.has_x() ? v.x() : 0.0;
    auto y = v.has_y() ? v.y() : 0.0;
    return fbs::osi3::CreateVector2d(builder, x, y);
}

::flatbuffers::Offset<fbs::osi3::Timestamp> ConvertTimestamp(const osi3::Timestamp& t, flatbuffers::FlatBufferBuilder& builder)
{
    auto seconds = t.has_seconds() ? t.seconds() : 0;
    auto nanos   = t.has_nanos() ? t.nanos() : 0;
    return fbs::osi3::CreateTimestamp(builder, seconds, nanos);
}

::flatbuffers::Offset<fbs::osi3::Dimension3d> ConvertDimension3d(const osi3::Dimension3d& d, flatbuffers::FlatBufferBuilder& builder)
{
    auto l = d.has_length() ? d.length() : 0.0;
    auto w = d.has_width() ? d.width() : 0.0;
    auto h = d.has_height() ? d.height() : 0.0;
    return fbs::osi3::CreateDimension3d(builder, l, w, h);
}

::flatbuffers::Offset<fbs::osi3::Orientation3d> ConvertOrientation3d(const osi3::Orientation3d& o, flatbuffers::FlatBufferBuilder& builder)
{
    auto r = o.has_roll() ? o.roll() : 0.0;
    auto p = o.has_pitch() ? o.pitch() : 0.0;
    auto y = o.has_yaw() ? o.yaw() : 0.0;
    return fbs::osi3::CreateOrientation3d(builder, r, p, y);
}

::flatbuffers::Offset<fbs::osi3::Identifier> ConvertIdentifier(const osi3::Identifier& i, flatbuffers::FlatBufferBuilder& builder)
{
    auto value = i.has_value() ? i.value() : 0;
    return fbs::osi3::CreateIdentifier(builder, value);
}

// ---------- Composite Types ----------

::flatbuffers::Offset<fbs::osi3::ExternalReference> ConvertExternalReference(const osi3::ExternalReference&  e,
                                                                             flatbuffers::FlatBufferBuilder& builder)
{
    auto reference = e.has_reference() ? builder.CreateString(e.reference()) : 0;
    auto type      = e.has_type() ? builder.CreateString(e.type()) : 0;

    std::vector<flatbuffers::Offset<flatbuffers::String>> ids;
    for (const auto& id : e.identifier())
        ids.push_back(builder.CreateString(id));

    auto identifiers = builder.CreateVector(ids);
    return fbs::osi3::CreateExternalReference(builder, reference, type, identifiers);
}

::flatbuffers::Offset<fbs::osi3::MountingPosition> ConvertMountingPosition(const osi3::MountingPosition& m, flatbuffers::FlatBufferBuilder& builder)
{
    auto pos = m.has_position() ? ConvertVector3d(m.position(), builder) : 0;
    auto ori = m.has_orientation() ? ConvertOrientation3d(m.orientation(), builder) : 0;
    return fbs::osi3::CreateMountingPosition(builder, pos, ori);
}

::flatbuffers::Offset<fbs::osi3::Spherical3d> ConvertSpherical3d(const osi3::Spherical3d& s, flatbuffers::FlatBufferBuilder& builder)
{
    auto d = s.has_distance() ? s.distance() : 0.0;
    auto a = s.has_azimuth() ? s.azimuth() : 0.0;
    auto e = s.has_elevation() ? s.elevation() : 0.0;
    return fbs::osi3::CreateSpherical3d(builder, d, a, e);
}

::flatbuffers::Offset<fbs::osi3::LogicalLaneAssignment> ConvertLogicalLaneAssignment(const osi3::LogicalLaneAssignment& l,
                                                                                     flatbuffers::FlatBufferBuilder&    builder)
{
    auto id = l.has_assigned_lane_id() ? ConvertIdentifier(l.assigned_lane_id(), builder) : 0;
    auto s  = l.has_s_position() ? l.s_position() : 0.0;
    auto t  = l.has_t_position() ? l.t_position() : 0.0;
    auto a  = l.has_angle_to_lane() ? l.angle_to_lane() : 0.0;
    return fbs::osi3::CreateLogicalLaneAssignment(builder, id, s, t, a);
}

::flatbuffers::Offset<fbs::osi3::BoundingBox> ConvertBoundingBox(const osi3::BoundingBox& b, flatbuffers::FlatBufferBuilder& builder)
{
    auto dim = b.has_dimension() ? ConvertDimension3d(b.dimension(), builder) : 0;

    auto pos = b.has_position() ? ConvertVector3d(b.position(), builder) : 0;

    auto ori = b.has_orientation() ? ConvertOrientation3d(b.orientation(), builder) : 0;

    auto type = b.has_contained_object_type() ? static_cast<fbs::osi3::BoundingBox_::Type>(b.contained_object_type())
                                              : static_cast<fbs::osi3::BoundingBox_::Type>(0);

    auto model = b.has_model_reference() ? builder.CreateString(b.model_reference()) : 0;

    return fbs::osi3::CreateBoundingBox(builder, dim, pos, ori, type, model);
}

// ---------- Base Objects ----------

::flatbuffers::Offset<fbs::osi3::BaseStationary> ConvertBaseStationary(const osi3::BaseStationary& b, flatbuffers::FlatBufferBuilder& builder)
{
    auto dim = b.has_dimension() ? ConvertDimension3d(b.dimension(), builder) : 0;
    auto pos = b.has_position() ? ConvertVector3d(b.position(), builder) : 0;
    auto ori = b.has_orientation() ? ConvertOrientation3d(b.orientation(), builder) : 0;

    std::vector<flatbuffers::Offset<fbs::osi3::Vector2d>> poly;
    for (const auto& p : b.base_polygon())
        poly.push_back(ConvertVector2d(p, builder));

    std::vector<flatbuffers::Offset<fbs::osi3::BoundingBox>> boxes;
    for (const auto& bb : b.bounding_box_section())
        boxes.push_back(ConvertBoundingBox(bb, builder));

    return fbs::osi3::CreateBaseStationary(builder, dim, pos, ori, builder.CreateVector(poly), builder.CreateVector(boxes));
}

::flatbuffers::Offset<fbs::osi3::BaseMoving> ConvertBaseMoving(const osi3::BaseMoving& b, flatbuffers::FlatBufferBuilder& builder)
{
    auto dim = b.has_dimension() ? ConvertDimension3d(b.dimension(), builder) : 0;
    auto pos = b.has_position() ? ConvertVector3d(b.position(), builder) : 0;
    auto ori = b.has_orientation() ? ConvertOrientation3d(b.orientation(), builder) : 0;
    auto vel = b.has_velocity() ? ConvertVector3d(b.velocity(), builder) : 0;
    auto acc = b.has_acceleration() ? ConvertVector3d(b.acceleration(), builder) : 0;
    auto orr = b.has_orientation_rate() ? ConvertOrientation3d(b.orientation_rate(), builder) : 0;
    auto ora = b.has_orientation_acceleration() ? ConvertOrientation3d(b.orientation_acceleration(), builder) : 0;

    std::vector<flatbuffers::Offset<fbs::osi3::Vector2d>> poly;
    for (const auto& p : b.base_polygon())
        poly.push_back(ConvertVector2d(p, builder));

    std::vector<flatbuffers::Offset<fbs::osi3::BoundingBox>> boxes;
    for (const auto& bb : b.bounding_box_section())
        boxes.push_back(ConvertBoundingBox(bb, builder));

    return fbs::osi3::CreateBaseMoving(builder, dim, pos, ori, vel, acc, orr, ora, builder.CreateVector(poly), builder.CreateVector(boxes));
}

::flatbuffers::Offset<fbs::osi3::Polygon3d> ConvertPolygon3d(const osi3::Polygon3d& p, flatbuffers::FlatBufferBuilder& builder)
{
    std::vector<flatbuffers::Offset<fbs::osi3::Vector3d>> vertices;
    for (const auto& v : p.vertex())
        vertices.push_back(ConvertVector3d(v, builder));

    return fbs::osi3::CreatePolygon3d(builder, builder.CreateVector(vertices));
}

// =====================
// StatePoint
// =====================

::flatbuffers::Offset<fbs::osi3::StatePoint> ConvertStatePoint(const osi3::StatePoint& s, flatbuffers::FlatBufferBuilder& builder)
{
    auto ts = s.has_timestamp() ? ConvertTimestamp(s.timestamp(), builder) : 0;

    auto pos = s.has_position() ? ConvertVector3d(s.position(), builder) : 0;

    auto ori = s.has_orientation() ? ConvertOrientation3d(s.orientation(), builder) : 0;

    return fbs::osi3::CreateStatePoint(builder, ts, pos, ori);
}

// =====================
// WavelengthData
// =====================

::flatbuffers::Offset<fbs::osi3::WavelengthData> ConvertWavelengthData(const osi3::WavelengthData& w, flatbuffers::FlatBufferBuilder& builder)
{
    auto start   = w.has_start() ? w.start() : 0.0;
    auto end     = w.has_end() ? w.end() : 0.0;
    auto samples = w.has_samples_number() ? w.samples_number() : 0.0;

    return fbs::osi3::CreateWavelengthData(builder, start, end, samples);
}

// =====================
// SpatialSignalStrength
// =====================

::flatbuffers::Offset<fbs::osi3::SpatialSignalStrength> ConvertSpatialSignalStrength(const osi3::SpatialSignalStrength& s,
                                                                                     flatbuffers::FlatBufferBuilder&    builder)
{
    auto h = s.has_horizontal_angle() ? s.horizontal_angle() : 0.0;
    auto v = s.has_vertical_angle() ? s.vertical_angle() : 0.0;
    auto p = s.has_signal_strength() ? s.signal_strength() : 0.0;

    return fbs::osi3::CreateSpatialSignalStrength(builder, h, v, p);
}

// =====================
// Color Converters
// =====================

::flatbuffers::Offset<fbs::osi3::ColorGrey> ConvertColorGrey(const osi3::ColorGrey& c, flatbuffers::FlatBufferBuilder& builder)
{
    auto g = c.has_grey() ? c.grey() : 0.0;
    return fbs::osi3::CreateColorGrey(builder, g);
}

::flatbuffers::Offset<fbs::osi3::ColorRGB> ConvertColorRGB(const osi3::ColorRGB& c, flatbuffers::FlatBufferBuilder& builder)
{
    auto r = c.has_red() ? c.red() : 0.0;
    auto g = c.has_green() ? c.green() : 0.0;
    auto b = c.has_blue() ? c.blue() : 0.0;

    return fbs::osi3::CreateColorRGB(builder, r, g, b);
}

::flatbuffers::Offset<fbs::osi3::ColorRGBIR> ConvertColorRGBIR(const osi3::ColorRGBIR& c, flatbuffers::FlatBufferBuilder& builder)
{
    auto r  = c.has_red() ? c.red() : 0.0;
    auto g  = c.has_green() ? c.green() : 0.0;
    auto b  = c.has_blue() ? c.blue() : 0.0;
    auto ir = c.has_infrared() ? c.infrared() : 0.0;

    return fbs::osi3::CreateColorRGBIR(builder, r, g, b, ir);
}

::flatbuffers::Offset<fbs::osi3::ColorHSV> ConvertColorHSV(const osi3::ColorHSV& c, flatbuffers::FlatBufferBuilder& builder)
{
    auto h = c.has_hue() ? c.hue() : 0.0;
    auto s = c.has_saturation() ? c.saturation() : 0.0;
    auto v = c.has_value() ? c.value() : 0.0;

    return fbs::osi3::CreateColorHSV(builder, h, s, v);
}

::flatbuffers::Offset<fbs::osi3::ColorLUV> ConvertColorLUV(const osi3::ColorLUV& c, flatbuffers::FlatBufferBuilder& builder)
{
    auto l = c.has_luminance() ? c.luminance() : 0.0;
    auto u = c.has_u() ? c.u() : 0.0;
    auto v = c.has_v() ? c.v() : 0.0;

    return fbs::osi3::CreateColorLUV(builder, l, u, v);
}

::flatbuffers::Offset<fbs::osi3::ColorCMYK> ConvertColorCMYK(const osi3::ColorCMYK& c, flatbuffers::FlatBufferBuilder& builder)
{
    auto cy = c.has_cyan() ? c.cyan() : 0.0;
    auto m  = c.has_magenta() ? c.magenta() : 0.0;
    auto y  = c.has_yellow() ? c.yellow() : 0.0;
    auto k  = c.has_key() ? c.key() : 0.0;

    return fbs::osi3::CreateColorCMYK(builder, cy, m, y, k);
}

// =====================
// Pedalry
// =====================

::flatbuffers::Offset<fbs::osi3::Pedalry> ConvertPedalry(const osi3::Pedalry& p, flatbuffers::FlatBufferBuilder& builder)
{
    auto acc = p.has_pedal_position_acceleration() ? p.pedal_position_acceleration() : 0.0;
    auto brk = p.has_pedal_position_brake() ? p.pedal_position_brake() : 0.0;
    auto clu = p.has_pedal_position_clutch() ? p.pedal_position_clutch() : 0.0;

    return fbs::osi3::CreatePedalry(builder, acc, brk, clu);
}

// =====================
// VehicleSteeringWheel
// =====================

::flatbuffers::Offset<fbs::osi3::VehicleSteeringWheel> ConvertVehicleSteeringWheel(const osi3::VehicleSteeringWheel& v,
                                                                                   flatbuffers::FlatBufferBuilder&   builder)
{
    auto a = v.has_angle() ? v.angle() : 0.0;
    auto s = v.has_angular_speed() ? v.angular_speed() : 0.0;
    auto t = v.has_torque() ? v.torque() : 0.0;

    return fbs::osi3::CreateVehicleSteeringWheel(builder, a, s, t);
}

// =====================
// GeodeticPosition
// =====================

::flatbuffers::Offset<fbs::osi3::GeodeticPosition> ConvertGeodeticPosition(const osi3::GeodeticPosition& g, flatbuffers::FlatBufferBuilder& builder)
{
    auto lon = g.has_longitude() ? g.longitude() : 0.0;
    auto lat = g.has_latitude() ? g.latitude() : 0.0;
    auto alt = g.has_altitude() ? g.altitude() : 0.0;

    return fbs::osi3::CreateGeodeticPosition(builder, lon, lat, alt);
}

::flatbuffers::Offset<fbs::osi3::ColorDescription> ConvertColorDescription(const osi3::ColorDescription& c, flatbuffers::FlatBufferBuilder& builder)
{
    auto grey = c.has_grey() ? ConvertColorGrey(c.grey(), builder) : 0;

    auto rgb = c.has_rgb() ? ConvertColorRGB(c.rgb(), builder) : 0;

    auto rgbir = c.has_rgbir() ? ConvertColorRGBIR(c.rgbir(), builder) : 0;

    auto hsv = c.has_hsv() ? ConvertColorHSV(c.hsv(), builder) : 0;

    auto luv = c.has_luv() ? ConvertColorLUV(c.luv(), builder) : 0;

    auto cmyk = c.has_cmyk() ? ConvertColorCMYK(c.cmyk(), builder) : 0;

    return fbs::osi3::CreateColorDescription(builder, grey, rgb, rgbir, hsv, luv, cmyk);
}

// osi_environment

// =====================
// EnvironmentalConditions Converters
// =====================

::flatbuffers::Offset<fbs::osi3::EnvironmentalConditions_::TimeOfDay> ConvertEnvironmentalConditionsTimeOfDay(
    const osi3::EnvironmentalConditions::TimeOfDay& t,
    flatbuffers::FlatBufferBuilder&                 builder)
{
    auto seconds = t.has_seconds_since_midnight() ? t.seconds_since_midnight() : 0;
    return fbs::osi3::EnvironmentalConditions_::CreateTimeOfDay(builder, seconds);
}

::flatbuffers::Offset<fbs::osi3::EnvironmentalConditions_::CloudLayer> ConvertEnvironmentalConditionsCloudLayer(
    const osi3::EnvironmentalConditions::CloudLayer& c,
    flatbuffers::FlatBufferBuilder&                  builder)
{
    auto cover = c.has_fractional_cloud_cover()
                     ? static_cast<fbs::osi3::EnvironmentalConditions_::CloudLayer_::FractionalCloudCover>(c.fractional_cloud_cover())
                     : static_cast<fbs::osi3::EnvironmentalConditions_::CloudLayer_::FractionalCloudCover>(0);

    return fbs::osi3::EnvironmentalConditions_::CreateCloudLayer(builder, cover);
}

::flatbuffers::Offset<fbs::osi3::EnvironmentalConditions_::Wind> ConvertEnvironmentalConditionsWind(const osi3::EnvironmentalConditions::Wind& w,
                                                                                                    flatbuffers::FlatBufferBuilder& builder)
{
    auto dir   = w.has_origin_direction() ? w.origin_direction() : 0.0;
    auto speed = w.has_speed() ? w.speed() : 0.0;

    return fbs::osi3::EnvironmentalConditions_::CreateWind(builder, dir, speed);
}

::flatbuffers::Offset<fbs::osi3::EnvironmentalConditions_::Sun> ConvertEnvironmentalConditionsSun(const osi3::EnvironmentalConditions::Sun& s,
                                                                                                  flatbuffers::FlatBufferBuilder&           builder)
{
    auto az = s.has_azimuth() ? s.azimuth() : 0.0;
    auto el = s.has_elevation() ? s.elevation() : 0.0;
    auto in = s.has_intensity() ? s.intensity() : 0.0;

    return fbs::osi3::EnvironmentalConditions_::CreateSun(builder, az, el, in);
}

::flatbuffers::Offset<fbs::osi3::EnvironmentalConditions> ConvertEnvironmentalConditions(const osi3::EnvironmentalConditions& e,
                                                                                         flatbuffers::FlatBufferBuilder&      builder)
{
    auto ambient = e.has_ambient_illumination() ? static_cast<fbs::osi3::EnvironmentalConditions_::AmbientIllumination>(e.ambient_illumination())
                                                : static_cast<fbs::osi3::EnvironmentalConditions_::AmbientIllumination>(0);

    auto precipitation = e.has_precipitation() ? static_cast<fbs::osi3::EnvironmentalConditions_::Precipitation>(e.precipitation())
                                               : static_cast<fbs::osi3::EnvironmentalConditions_::Precipitation>(0);

    auto fog =
        e.has_fog() ? static_cast<fbs::osi3::EnvironmentalConditions_::Fog>(e.fog()) : static_cast<fbs::osi3::EnvironmentalConditions_::Fog>(0);

    auto time_of_day = e.has_time_of_day() ? ConvertEnvironmentalConditionsTimeOfDay(e.time_of_day(), builder) : 0;

    auto clouds = e.has_clouds() ? ConvertEnvironmentalConditionsCloudLayer(e.clouds(), builder) : 0;

    auto wind = e.has_wind() ? ConvertEnvironmentalConditionsWind(e.wind(), builder) : 0;

    auto sun = e.has_sun() ? ConvertEnvironmentalConditionsSun(e.sun(), builder) : 0;

    auto unix_ts  = e.has_unix_timestamp() ? e.unix_timestamp() : 0;
    auto pressure = e.has_atmospheric_pressure() ? e.atmospheric_pressure() : 0.0;
    auto temp     = e.has_temperature() ? e.temperature() : 0.0;
    auto humidity = e.has_relative_humidity() ? e.relative_humidity() : 0.0;

    std::vector<flatbuffers::Offset<fbs::osi3::ExternalReference>> refs;
    for (const auto& r : e.source_reference())
        refs.push_back(ConvertExternalReference(r, builder));

    auto source_refs = builder.CreateVector(refs);

    return fbs::osi3::CreateEnvironmentalConditions(builder,
                                                    ambient,
                                                    time_of_day,
                                                    unix_ts,
                                                    pressure,
                                                    temp,
                                                    humidity,
                                                    precipitation,
                                                    fog,
                                                    source_refs,
                                                    clouds,
                                                    wind,
                                                    sun);
}

// osi_lane

// =====================
// Lane Converters
// =====================

::flatbuffers::Offset<fbs::osi3::Lane_::Classification_::RoadCondition> ConvertLaneRoadCondition(const osi3::Lane::Classification::RoadCondition& r,
                                                                                                 flatbuffers::FlatBufferBuilder& builder)
{
    auto temp   = r.has_surface_temperature() ? r.surface_temperature() : 0.0;
    auto water  = r.has_surface_water_film() ? r.surface_water_film() : 0.0;
    auto freeze = r.has_surface_freezing_point() ? r.surface_freezing_point() : 0.0;
    auto ice    = r.has_surface_ice() ? r.surface_ice() : 0.0;
    auto rough  = r.has_surface_roughness() ? r.surface_roughness() : 0.0;
    auto text   = r.has_surface_texture() ? r.surface_texture() : 0.0;

    return fbs::osi3::Lane_::Classification_::CreateRoadCondition(builder, temp, water, freeze, ice, rough, text);
}

::flatbuffers::Offset<fbs::osi3::Lane_::Classification_::LanePairing> ConvertLanePairing(const osi3::Lane::Classification::LanePairing& p,
                                                                                         flatbuffers::FlatBufferBuilder&                builder)
{
    auto ant = p.has_antecessor_lane_id() ? ConvertIdentifier(p.antecessor_lane_id(), builder) : 0;
    auto suc = p.has_successor_lane_id() ? ConvertIdentifier(p.successor_lane_id(), builder) : 0;

    return fbs::osi3::Lane_::Classification_::CreateLanePairing(builder, ant, suc);
}

::flatbuffers::Offset<fbs::osi3::Lane_::Classification> ConvertLaneClassification(const osi3::Lane::Classification& c,
                                                                                  flatbuffers::FlatBufferBuilder&   builder)
{
    auto type =
        c.has_type() ? static_cast<fbs::osi3::Lane_::Classification_::Type>(c.type()) : static_cast<fbs::osi3::Lane_::Classification_::Type>(0);

    auto subtype = c.has_subtype() ? static_cast<fbs::osi3::Lane_::Classification_::Subtype>(c.subtype())
                                   : static_cast<fbs::osi3::Lane_::Classification_::Subtype>(0);

    auto is_host = c.has_is_host_vehicle_lane() ? c.is_host_vehicle_lane() : false;

    auto dir = c.has_centerline_is_driving_direction() ? c.centerline_is_driving_direction() : false;

    std::vector<flatbuffers::Offset<fbs::osi3::Vector3d>> centerline;
    for (const auto& v : c.centerline())
        centerline.push_back(ConvertVector3d(v, builder));

    std::vector<flatbuffers::Offset<fbs::osi3::Identifier>> left_adj;
    for (const auto& id : c.left_adjacent_lane_id())
        left_adj.push_back(ConvertIdentifier(id, builder));

    std::vector<flatbuffers::Offset<fbs::osi3::Identifier>> right_adj;
    for (const auto& id : c.right_adjacent_lane_id())
        right_adj.push_back(ConvertIdentifier(id, builder));

    std::vector<flatbuffers::Offset<fbs::osi3::Lane_::Classification_::LanePairing>> pairings;
    for (const auto& p : c.lane_pairing())
        pairings.push_back(ConvertLanePairing(p, builder));

    std::vector<flatbuffers::Offset<fbs::osi3::Identifier>> right_bound;
    for (const auto& id : c.right_lane_boundary_id())
        right_bound.push_back(ConvertIdentifier(id, builder));

    std::vector<flatbuffers::Offset<fbs::osi3::Identifier>> left_bound;
    for (const auto& id : c.left_lane_boundary_id())
        left_bound.push_back(ConvertIdentifier(id, builder));

    std::vector<flatbuffers::Offset<fbs::osi3::Identifier>> free_bound;
    for (const auto& id : c.free_lane_boundary_id())
        free_bound.push_back(ConvertIdentifier(id, builder));

    auto road = c.has_road_condition() ? ConvertLaneRoadCondition(c.road_condition(), builder) : 0;

    return fbs::osi3::Lane_::CreateClassification(builder,
                                                  type,
                                                  is_host,
                                                  builder.CreateVector(centerline),
                                                  dir,
                                                  builder.CreateVector(left_adj),
                                                  builder.CreateVector(right_adj),
                                                  builder.CreateVector(pairings),
                                                  builder.CreateVector(right_bound),
                                                  builder.CreateVector(left_bound),
                                                  builder.CreateVector(free_bound),
                                                  road,
                                                  subtype);
}

::flatbuffers::Offset<fbs::osi3::Lane> ConvertLane(const osi3::Lane& l, flatbuffers::FlatBufferBuilder& builder)
{
    auto id = l.has_id() ? ConvertIdentifier(l.id(), builder) : 0;

    auto cls = l.has_classification() ? ConvertLaneClassification(l.classification(), builder) : 0;

    std::vector<flatbuffers::Offset<fbs::osi3::ExternalReference>> refs;
    for (const auto& r : l.source_reference())
        refs.push_back(ConvertExternalReference(r, builder));

    return fbs::osi3::CreateLane(builder, id, cls, builder.CreateVector(refs));
}

// =====================
// LaneBoundary Converters
// =====================

::flatbuffers::Offset<fbs::osi3::LaneBoundary_::BoundaryPoint> ConvertLaneBoundaryPoint(const osi3::LaneBoundary::BoundaryPoint& b,
                                                                                        flatbuffers::FlatBufferBuilder&          builder)
{
    auto pos = b.has_position() ? ConvertVector3d(b.position(), builder) : 0;

    auto width  = b.has_width() ? b.width() : 0.0;
    auto height = b.has_height() ? b.height() : 0.0;

    auto dash = b.has_dash() ? static_cast<fbs::osi3::LaneBoundary_::BoundaryPoint_::Dash>(b.dash())
                             : static_cast<fbs::osi3::LaneBoundary_::BoundaryPoint_::Dash>(0);

    return fbs::osi3::LaneBoundary_::CreateBoundaryPoint(builder, pos, width, height, dash);
}

::flatbuffers::Offset<fbs::osi3::LaneBoundary_::Classification> ConvertLaneBoundaryClassification(const osi3::LaneBoundary::Classification& c,
                                                                                                  flatbuffers::FlatBufferBuilder&           builder)
{
    auto type = c.has_type() ? static_cast<fbs::osi3::LaneBoundary_::Classification_::Type>(c.type())
                             : static_cast<fbs::osi3::LaneBoundary_::Classification_::Type>(0);

    auto color = c.has_color() ? static_cast<fbs::osi3::LaneBoundary_::Classification_::Color>(c.color())
                               : static_cast<fbs::osi3::LaneBoundary_::Classification_::Color>(0);

    std::vector<flatbuffers::Offset<fbs::osi3::Identifier>> limits;
    for (const auto& id : c.limiting_structure_id())
    {
        limits.push_back(ConvertIdentifier(id, builder));
    }

    return fbs::osi3::LaneBoundary_::CreateClassification(builder, type, color, builder.CreateVector(limits));
}

::flatbuffers::Offset<fbs::osi3::LaneBoundary> ConvertLaneBoundary(const osi3::LaneBoundary& b, flatbuffers::FlatBufferBuilder& builder)
{
    auto id = b.has_id() ? ConvertIdentifier(b.id(), builder) : 0;

    std::vector<flatbuffers::Offset<fbs::osi3::LaneBoundary_::BoundaryPoint>> line;
    for (const auto& p : b.boundary_line())
        line.push_back(ConvertLaneBoundaryPoint(p, builder));

    auto cls = b.has_classification() ? ConvertLaneBoundaryClassification(b.classification(), builder) : 0;

    std::vector<flatbuffers::Offset<fbs::osi3::ExternalReference>> refs;
    for (const auto& r : b.source_reference())
        refs.push_back(ConvertExternalReference(r, builder));

    auto color = b.has_color_description() ? ConvertColorDescription(b.color_description(), builder) : 0;

    return fbs::osi3::CreateLaneBoundary(builder, id, builder.CreateVector(line), cls, builder.CreateVector(refs), color);
}

// osi_object

// ==========================
// StationaryObject
// ==========================

::flatbuffers::Offset<fbs::osi3::StationaryObject_::Classification_::EmittingStructureAttribute> ConvertEmittingStructureAttribute(
    const osi3::StationaryObject::Classification::EmittingStructureAttribute& proto,
    flatbuffers::FlatBufferBuilder&                                           builder)
{
    fbs::osi3::StationaryObject_::Classification_::EmittingStructureAttributeBuilder b(builder);

    std::vector<::flatbuffers::Offset<fbs::osi3::WavelengthData>> wavelength_data;
    for (const auto& w : proto.wavelength_data())
    {
        wavelength_data.push_back(ConvertWavelengthData(w, builder));
    }

    std::vector<::flatbuffers::Offset<fbs::osi3::SpatialSignalStrength>> emitted_strength;
    for (const auto& s : proto.emitted_spatial_signal_strength())
    {
        emitted_strength.push_back(ConvertSpatialSignalStrength(s, builder));
    }

    return fbs::osi3::StationaryObject_::Classification_::CreateEmittingStructureAttribute(builder,
                                                                                           builder.CreateVector(wavelength_data),
                                                                                           builder.CreateVector(emitted_strength));
}

::flatbuffers::Offset<fbs::osi3::StationaryObject_::Classification> ConvertStationaryObjectClassification(
    const osi3::StationaryObject::Classification& proto,
    flatbuffers::FlatBufferBuilder&               builder)
{
    fbs::osi3::StationaryObject_::ClassificationBuilder b(builder);

    auto type = proto.has_type() ? static_cast<fbs::osi3::StationaryObject_::Classification_::Type>(proto.type())
                                 : static_cast<fbs::osi3::StationaryObject_::Classification_::Type>(0);

    auto material = proto.has_material() ? static_cast<fbs::osi3::StationaryObject_::Classification_::Material>(proto.material())
                                         : static_cast<fbs::osi3::StationaryObject_::Classification_::Material>(0);

    auto density = proto.has_density() ? static_cast<fbs::osi3::StationaryObject_::Classification_::Density>(proto.density())
                                       : static_cast<fbs::osi3::StationaryObject_::Classification_::Density>(0);

    auto color = proto.has_color() ? static_cast<fbs::osi3::StationaryObject_::Classification_::Color>(proto.color())
                                   : static_cast<fbs::osi3::StationaryObject_::Classification_::Color>(0);

    auto emitting = proto.has_emitting_structure_attribute() ? ConvertEmittingStructureAttribute(proto.emitting_structure_attribute(), builder) : 0;

    std::vector<::flatbuffers::Offset<fbs::osi3::Identifier>> assigned_lane_id;
    for (const auto& id : proto.assigned_lane_id())
    {
        assigned_lane_id.push_back(ConvertIdentifier(id, builder));
    }

    std::vector<double> assigned_lane_percentage;
    for (double p : proto.assigned_lane_percentage())
    {
        assigned_lane_percentage.push_back(p);
    }

    std::vector<::flatbuffers::Offset<fbs::osi3::LogicalLaneAssignment>> logical_lane_assignment;
    for (const auto& l : proto.logical_lane_assignment())
    {
        logical_lane_assignment.push_back(ConvertLogicalLaneAssignment(l, builder));
    }

    return fbs::osi3::StationaryObject_::CreateClassification(builder,
                                                              type,
                                                              material,
                                                              density,
                                                              color,
                                                              emitting,
                                                              builder.CreateVector(assigned_lane_id),
                                                              builder.CreateVector(assigned_lane_percentage),
                                                              builder.CreateVector(logical_lane_assignment));
}

::flatbuffers::Offset<fbs::osi3::StationaryObject> ConvertStationaryObject(const osi3::StationaryObject&   proto,
                                                                           flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::StationaryObjectBuilder b(builder);

    auto id             = proto.has_id() ? ConvertIdentifier(proto.id(), builder) : 0;
    auto base           = proto.has_base() ? ConvertBaseStationary(proto.base(), builder) : 0;
    auto classification = proto.has_classification() ? ConvertStationaryObjectClassification(proto.classification(), builder) : 0;

    auto model_reference = proto.has_model_reference() ? builder.CreateString(proto.model_reference()) : 0;

    std::vector<::flatbuffers::Offset<fbs::osi3::ExternalReference>> source_reference;
    for (const auto& s : proto.source_reference())
    {
        source_reference.push_back(ConvertExternalReference(s, builder));
    }

    auto color_description = proto.has_color_description() ? ConvertColorDescription(proto.color_description(), builder) : 0;

    return fbs::osi3::CreateStationaryObject(builder,
                                             id,
                                             base,
                                             classification,
                                             model_reference,
                                             builder.CreateVector(source_reference),
                                             color_description);
}

// ==========================
// MovingObject - Pedestrian
// ==========================

::flatbuffers::Offset<fbs::osi3::MovingObject_::PedestrianAttributes_::Bone> ConvertPedestrianBone(
    const osi3::MovingObject::PedestrianAttributes::Bone& proto,
    flatbuffers::FlatBufferBuilder&                       builder)
{
    fbs::osi3::MovingObject_::PedestrianAttributes_::BoneBuilder b(builder);

    auto type = proto.has_type() ? static_cast<fbs::osi3::MovingObject_::PedestrianAttributes_::Bone_::Type>(proto.type())
                                 : static_cast<fbs::osi3::MovingObject_::PedestrianAttributes_::Bone_::Type>(0);

    auto position         = proto.has_position() ? ConvertVector3d(proto.position(), builder) : 0;
    auto orientation      = proto.has_orientation() ? ConvertOrientation3d(proto.orientation(), builder) : 0;
    auto length           = proto.has_length() ? proto.length() : 0.0f;
    auto missing          = proto.has_missing() ? proto.missing() : false;
    auto velocity         = proto.has_velocity() ? ConvertVector3d(proto.velocity(), builder) : 0;
    auto orientation_rate = proto.has_orientation_rate() ? ConvertOrientation3d(proto.orientation_rate(), builder) : 0;

    return fbs::osi3::MovingObject_::PedestrianAttributes_::CreateBone(builder,
                                                                       type,
                                                                       position,
                                                                       orientation,
                                                                       length,
                                                                       missing,
                                                                       velocity,
                                                                       orientation_rate);
}

::flatbuffers::Offset<fbs::osi3::MovingObject_::PedestrianAttributes> ConvertPedestrianAttributes(
    const osi3::MovingObject::PedestrianAttributes& proto,
    flatbuffers::FlatBufferBuilder&                 builder)
{
    fbs::osi3::MovingObject_::PedestrianAttributesBuilder b(builder);

    auto bbcenter_to_root = proto.has_bbcenter_to_root() ? ConvertVector3d(proto.bbcenter_to_root(), builder) : 0;

    std::vector<::flatbuffers::Offset<fbs::osi3::MovingObject_::PedestrianAttributes_::Bone>> bones;
    for (const auto& bone : proto.skeleton_bone())
    {
        bones.push_back(ConvertPedestrianBone(bone, builder));
    }

    return fbs::osi3::MovingObject_::CreatePedestrianAttributes(builder, bbcenter_to_root, builder.CreateVector(bones));
}

// ==========================
// MovingObject - Vehicle
// ==========================

::flatbuffers::Offset<fbs::osi3::MovingObject_::VehicleAttributes_::WheelData> ConvertWheelData(
    const osi3::MovingObject::VehicleAttributes::WheelData& proto,
    flatbuffers::FlatBufferBuilder&                         builder)
{
    fbs::osi3::MovingObject_::VehicleAttributes_::WheelDataBuilder b(builder);

    auto axle            = proto.has_axle() ? proto.axle() : 0;
    auto index           = proto.has_index() ? proto.index() : 0;
    auto position        = proto.has_position() ? ConvertVector3d(proto.position(), builder) : 0;
    auto wheel_radius    = proto.has_wheel_radius() ? proto.wheel_radius() : 0.0;
    auto rim_radius      = proto.has_rim_radius() ? proto.rim_radius() : 0.0;
    auto width           = proto.has_width() ? proto.width() : 0.0;
    auto orientation     = proto.has_orientation() ? ConvertOrientation3d(proto.orientation(), builder) : 0;
    auto rotation_rate   = proto.has_rotation_rate() ? proto.rotation_rate() : 0.0;
    auto model_reference = proto.has_model_reference() ? builder.CreateString(proto.model_reference()) : 0;
    auto friction        = proto.has_friction_coefficient() ? proto.friction_coefficient() : 0.0;

    return fbs::osi3::MovingObject_::VehicleAttributes_::CreateWheelData(builder,
                                                                         axle,
                                                                         index,
                                                                         position,
                                                                         wheel_radius,
                                                                         rim_radius,
                                                                         width,
                                                                         orientation,
                                                                         rotation_rate,
                                                                         model_reference,
                                                                         friction);
}

::flatbuffers::Offset<fbs::osi3::MovingObject_::VehicleAttributes> ConvertVehicleAttributes(const osi3::MovingObject::VehicleAttributes& proto,
                                                                                            flatbuffers::FlatBufferBuilder&              builder)
{
    fbs::osi3::MovingObject_::VehicleAttributesBuilder b(builder);

    auto driver_id         = proto.has_driver_id() ? ConvertIdentifier(proto.driver_id(), builder) : 0;
    auto radius_wheel      = proto.has_radius_wheel() ? proto.radius_wheel() : 0.0;
    auto number_wheels     = proto.has_number_wheels() ? proto.number_wheels() : 0;
    auto bbcenter_to_rear  = proto.has_bbcenter_to_rear() ? ConvertVector3d(proto.bbcenter_to_rear(), builder) : 0;
    auto bbcenter_to_front = proto.has_bbcenter_to_front() ? ConvertVector3d(proto.bbcenter_to_front(), builder) : 0;
    auto ground_clearance  = proto.has_ground_clearance() ? proto.ground_clearance() : 0.0;

    std::vector<::flatbuffers::Offset<fbs::osi3::MovingObject_::VehicleAttributes_::WheelData>> wheels;
    for (const auto& w : proto.wheel_data())
    {
        wheels.push_back(ConvertWheelData(w, builder));
    }

    auto steering_wheel_angle = proto.has_steering_wheel_angle() ? proto.steering_wheel_angle() : 0.0;

    return fbs::osi3::MovingObject_::CreateVehicleAttributes(builder,
                                                             driver_id,
                                                             radius_wheel,
                                                             number_wheels,
                                                             bbcenter_to_rear,
                                                             bbcenter_to_front,
                                                             ground_clearance,
                                                             builder.CreateVector(wheels),
                                                             steering_wheel_angle);
}

// ==========================
// MovingObject - Classification
// ==========================

::flatbuffers::Offset<fbs::osi3::MovingObject_::VehicleClassification_::LightState> ConvertLightState(
    const osi3::MovingObject::VehicleClassification::LightState& proto,
    flatbuffers::FlatBufferBuilder&                              builder)
{
    fbs::osi3::MovingObject_::VehicleClassification_::LightStateBuilder b(builder);

    auto indicator = proto.has_indicator_state()
                         ? static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::IndicatorState>(proto.indicator_state())
                         : static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::IndicatorState>(0);

    auto front_fog = proto.has_front_fog_light()
                         ? static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(proto.front_fog_light())
                         : static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(0);

    auto rear_fog = proto.has_rear_fog_light()
                        ? static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(proto.rear_fog_light())
                        : static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(0);

    auto head_light = proto.has_head_light()
                          ? static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(proto.head_light())
                          : static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(0);

    auto high_beam = proto.has_high_beam()
                         ? static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(proto.high_beam())
                         : static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(0);

    auto reversing = proto.has_reversing_light()
                         ? static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(proto.reversing_light())
                         : static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(0);

    auto tail = proto.has_tail_light()
                    ? static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(proto.tail_light())
                    : static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(0);

    auto brake = proto.has_brake_light_state()
                     ? static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::BrakeLightState>(proto.brake_light_state())
                     : static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::BrakeLightState>(0);

    auto license =
        proto.has_license_plate_illumination_rear()
            ? static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(proto.license_plate_illumination_rear())
            : static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(0);

    auto emergency =
        proto.has_emergency_vehicle_illumination()
            ? static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(proto.emergency_vehicle_illumination())
            : static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(0);

    auto service =
        proto.has_service_vehicle_illumination()
            ? static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(proto.service_vehicle_illumination())
            : static_cast<fbs::osi3::MovingObject_::VehicleClassification_::LightState_::GenericLightState>(0);

    return fbs::osi3::MovingObject_::VehicleClassification_::CreateLightState(builder,
                                                                              indicator,
                                                                              front_fog,
                                                                              rear_fog,
                                                                              head_light,
                                                                              high_beam,
                                                                              reversing,
                                                                              tail,
                                                                              brake,
                                                                              license,
                                                                              emergency,
                                                                              service);
}

// ===================== MovingObject::MovingObjectClassification =====================

::flatbuffers::Offset<fbs::osi3::MovingObject_::MovingObjectClassification> ConvertMovingObjectClassification(
    const osi3::MovingObject::MovingObjectClassification& cls,
    flatbuffers::FlatBufferBuilder&                       builder)
{
    fbs::osi3::MovingObject_::MovingObjectClassificationBuilder b(builder);

    std::vector<flatbuffers::Offset<fbs::osi3::Identifier>> assigned_lane_ids;
    for (const auto& id : cls.assigned_lane_id())
    {
        assigned_lane_ids.push_back(ConvertIdentifier(id, builder));
    }
    auto assigned_lane_id = builder.CreateVector(assigned_lane_ids);

    std::vector<double> assigned_lane_percentages;
    for (const auto& p : cls.assigned_lane_percentage())
    {
        assigned_lane_percentages.push_back(p);
    }
    auto assigned_lane_percentage = builder.CreateVector(assigned_lane_percentages);

    std::vector<flatbuffers::Offset<fbs::osi3::LogicalLaneAssignment>> logical_lane_assignments;
    for (const auto& lla : cls.logical_lane_assignment())
    {
        logical_lane_assignments.push_back(ConvertLogicalLaneAssignment(lla, builder));
    }
    auto logical_lane_assignment = builder.CreateVector(logical_lane_assignments);

    return fbs::osi3::MovingObject_::CreateMovingObjectClassification(builder, assigned_lane_id, assigned_lane_percentage, logical_lane_assignment);
}

// ===================== MovingObject::VehicleClassification =====================

::flatbuffers::Offset<fbs::osi3::MovingObject_::VehicleClassification> ConvertVehicleClassification(
    const osi3::MovingObject::VehicleClassification& cls,
    flatbuffers::FlatBufferBuilder&                  builder)
{
    fbs::osi3::MovingObject_::VehicleClassificationBuilder b(builder);

    auto type = cls.has_type() ? static_cast<fbs::osi3::MovingObject_::VehicleClassification_::Type>(cls.type())
                               : static_cast<fbs::osi3::MovingObject_::VehicleClassification_::Type>(0);

    auto light_state = cls.has_light_state() ? ConvertLightState(cls.light_state(), builder)
                                             : flatbuffers::Offset<fbs::osi3::MovingObject_::VehicleClassification_::LightState>();

    auto has_trailer = cls.has_has_trailer() ? cls.has_trailer() : false;

    auto trailer_id = cls.has_trailer_id() ? ConvertIdentifier(cls.trailer_id(), builder) : flatbuffers::Offset<fbs::osi3::Identifier>();

    auto role = cls.has_role() ? static_cast<fbs::osi3::MovingObject_::VehicleClassification_::Role>(cls.role())
                               : static_cast<fbs::osi3::MovingObject_::VehicleClassification_::Role>(0);

    return fbs::osi3::MovingObject_::CreateVehicleClassification(builder, type, light_state, has_trailer, trailer_id, role);
}

// ===================== MovingObject =====================

::flatbuffers::Offset<fbs::osi3::MovingObject> ConvertMovingObject(const osi3::MovingObject& obj, flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::MovingObjectBuilder b(builder);

    auto id = obj.has_id() ? ConvertIdentifier(obj.id(), builder) : flatbuffers::Offset<fbs::osi3::Identifier>();

    auto base = obj.has_base() ? ConvertBaseMoving(obj.base(), builder) : flatbuffers::Offset<fbs::osi3::BaseMoving>();

    auto type = obj.has_type() ? static_cast<fbs::osi3::MovingObject_::Type>(obj.type()) : static_cast<fbs::osi3::MovingObject_::Type>(0);

    std::vector<flatbuffers::Offset<fbs::osi3::Identifier>> assigned_lane_ids;
    for (const auto& idl : obj.assigned_lane_id())
    {
        assigned_lane_ids.push_back(ConvertIdentifier(idl, builder));
    }
    auto assigned_lane_id = builder.CreateVector(assigned_lane_ids);

    auto vehicle_attributes = obj.has_vehicle_attributes() ? ConvertVehicleAttributes(obj.vehicle_attributes(), builder)
                                                           : flatbuffers::Offset<fbs::osi3::MovingObject_::VehicleAttributes>();

    auto vehicle_classification = obj.has_vehicle_classification() ? ConvertVehicleClassification(obj.vehicle_classification(), builder)
                                                                   : flatbuffers::Offset<fbs::osi3::MovingObject_::VehicleClassification>();

    auto model_reference = obj.has_model_reference() ? builder.CreateString(obj.model_reference()) : flatbuffers::Offset<flatbuffers::String>();

    std::vector<flatbuffers::Offset<fbs::osi3::StatePoint>> future_trajectory;
    for (const auto& sp : obj.future_trajectory())
    {
        future_trajectory.push_back(ConvertStatePoint(sp, builder));
    }
    auto future_trajectory_vec = builder.CreateVector(future_trajectory);

    auto moving_object_classification = obj.has_moving_object_classification()
                                            ? ConvertMovingObjectClassification(obj.moving_object_classification(), builder)
                                            : flatbuffers::Offset<fbs::osi3::MovingObject_::MovingObjectClassification>();

    std::vector<flatbuffers::Offset<fbs::osi3::ExternalReference>> source_refs;
    for (const auto& ref : obj.source_reference())
    {
        source_refs.push_back(ConvertExternalReference(ref, builder));
    }
    auto source_reference = builder.CreateVector(source_refs);

    auto color_description =
        obj.has_color_description() ? ConvertColorDescription(obj.color_description(), builder) : flatbuffers::Offset<fbs::osi3::ColorDescription>();

    auto pedestrian_attributes = obj.has_pedestrian_attributes() ? ConvertPedestrianAttributes(obj.pedestrian_attributes(), builder)
                                                                 : flatbuffers::Offset<fbs::osi3::MovingObject_::PedestrianAttributes>();

    return fbs::osi3::CreateMovingObject(builder,
                                         id,
                                         base,
                                         type,
                                         assigned_lane_id,
                                         vehicle_attributes,
                                         vehicle_classification,
                                         model_reference,
                                         future_trajectory_vec,
                                         moving_object_classification,
                                         source_reference,
                                         color_description,
                                         pedestrian_attributes);
}

// osi_occupant

// ==========================
// Occupant - Classification
// ==========================

::flatbuffers::Offset<fbs::osi3::Occupant_::Classification> ConvertOccupantClassification(const osi3::Occupant::Classification& proto,
                                                                                          flatbuffers::FlatBufferBuilder&       builder)
{
    fbs::osi3::Occupant_::ClassificationBuilder b(builder);

    auto is_driver = proto.has_is_driver() ? proto.is_driver() : false;

    auto seat = proto.has_seat() ? static_cast<fbs::osi3::Occupant_::Classification_::Seat>(proto.seat())
                                 : static_cast<fbs::osi3::Occupant_::Classification_::Seat>(0);

    auto steering_control = proto.has_steering_control()
                                ? static_cast<fbs::osi3::Occupant_::Classification_::SteeringControl>(proto.steering_control())
                                : static_cast<fbs::osi3::Occupant_::Classification_::SteeringControl>(0);

    return fbs::osi3::Occupant_::CreateClassification(builder, is_driver, seat, steering_control);
}

// ==========================
// Occupant
// ==========================

::flatbuffers::Offset<fbs::osi3::Occupant> ConvertOccupant(const osi3::Occupant& proto, flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::OccupantBuilder b(builder);

    auto id = proto.has_id() ? ConvertIdentifier(proto.id(), builder) : 0;

    auto classification = proto.has_classification() ? ConvertOccupantClassification(proto.classification(), builder) : 0;

    std::vector<::flatbuffers::Offset<fbs::osi3::ExternalReference>> source_reference;
    for (const auto& s : proto.source_reference())
    {
        source_reference.push_back(ConvertExternalReference(s, builder));
    }

    return fbs::osi3::CreateOccupant(builder, id, classification, builder.CreateVector(source_reference));
}

// osi_referenceline

// ==========================
// ReferenceLinePoint
// ==========================

::flatbuffers::Offset<fbs::osi3::ReferenceLine_::ReferenceLinePoint> ConvertReferenceLinePoint(const osi3::ReferenceLine::ReferenceLinePoint& proto,
                                                                                               flatbuffers::FlatBufferBuilder&                builder)
{
    fbs::osi3::ReferenceLine_::ReferenceLinePointBuilder b(builder);

    auto world_position = proto.has_world_position() ? ConvertVector3d(proto.world_position(), builder) : 0;

    auto s_position = proto.has_s_position() ? proto.s_position() : 0.0;
    auto t_axis_yaw = proto.has_t_axis_yaw() ? proto.t_axis_yaw() : 0.0;

    return fbs::osi3::ReferenceLine_::CreateReferenceLinePoint(builder, world_position, s_position, t_axis_yaw);
}

// ==========================
// ReferenceLine
// ==========================

::flatbuffers::Offset<fbs::osi3::ReferenceLine> ConvertReferenceLine(const osi3::ReferenceLine& proto, flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::ReferenceLineBuilder b(builder);

    auto id = proto.has_id() ? ConvertIdentifier(proto.id(), builder) : 0;

    auto type = proto.has_type() ? static_cast<fbs::osi3::ReferenceLine_::Type>(proto.type()) : static_cast<fbs::osi3::ReferenceLine_::Type>(0);

    std::vector<::flatbuffers::Offset<fbs::osi3::ReferenceLine_::ReferenceLinePoint>> poly_line;
    for (const auto& p : proto.poly_line())
    {
        poly_line.push_back(ConvertReferenceLinePoint(p, builder));
    }

    return fbs::osi3::CreateReferenceLine(builder, id, type, builder.CreateVector(poly_line));
}

// osi_trafficlight

// ==========================
// TrafficLight - Classification
// ==========================

::flatbuffers::Offset<fbs::osi3::TrafficLight_::Classification> ConvertTrafficLightClassification(const osi3::TrafficLight::Classification& proto,
                                                                                                  flatbuffers::FlatBufferBuilder&           builder)
{
    fbs::osi3::TrafficLight_::ClassificationBuilder b(builder);

    auto color = proto.has_color() ? static_cast<fbs::osi3::TrafficLight_::Classification_::Color>(proto.color())
                                   : static_cast<fbs::osi3::TrafficLight_::Classification_::Color>(0);

    auto icon = proto.has_icon() ? static_cast<fbs::osi3::TrafficLight_::Classification_::Icon>(proto.icon())
                                 : static_cast<fbs::osi3::TrafficLight_::Classification_::Icon>(0);

    auto mode = proto.has_mode() ? static_cast<fbs::osi3::TrafficLight_::Classification_::Mode>(proto.mode())
                                 : static_cast<fbs::osi3::TrafficLight_::Classification_::Mode>(0);

    auto counter = proto.has_counter() ? proto.counter() : 0.0;

    std::vector<::flatbuffers::Offset<fbs::osi3::Identifier>> assigned_lane_id;
    for (const auto& id : proto.assigned_lane_id())
    {
        assigned_lane_id.push_back(ConvertIdentifier(id, builder));
    }

    auto is_out_of_service = proto.has_is_out_of_service() ? proto.is_out_of_service() : false;

    std::vector<::flatbuffers::Offset<fbs::osi3::LogicalLaneAssignment>> logical_lane_assignment;
    for (const auto& l : proto.logical_lane_assignment())
    {
        logical_lane_assignment.push_back(ConvertLogicalLaneAssignment(l, builder));
    }

    return fbs::osi3::TrafficLight_::CreateClassification(builder,
                                                          color,
                                                          icon,
                                                          mode,
                                                          counter,
                                                          builder.CreateVector(assigned_lane_id),
                                                          is_out_of_service,
                                                          builder.CreateVector(logical_lane_assignment));
}

// ==========================
// TrafficLight
// ==========================

::flatbuffers::Offset<fbs::osi3::TrafficLight> ConvertTrafficLight(const osi3::TrafficLight& proto, flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::TrafficLightBuilder b(builder);

    auto id = proto.has_id() ? ConvertIdentifier(proto.id(), builder) : 0;

    auto base = proto.has_base() ? ConvertBaseStationary(proto.base(), builder) : 0;

    auto classification = proto.has_classification() ? ConvertTrafficLightClassification(proto.classification(), builder) : 0;

    auto model_reference = proto.has_model_reference() ? builder.CreateString(proto.model_reference()) : 0;

    std::vector<::flatbuffers::Offset<fbs::osi3::ExternalReference>> source_reference;
    for (const auto& s : proto.source_reference())
    {
        source_reference.push_back(ConvertExternalReference(s, builder));
    }

    auto color_description = proto.has_color_description() ? ConvertColorDescription(proto.color_description(), builder) : 0;

    return fbs::osi3::CreateTrafficLight(builder,
                                         id,
                                         base,
                                         classification,
                                         model_reference,
                                         builder.CreateVector(source_reference),
                                         color_description);
}

// osi_trafficsign
// ===================== TrafficSignValue =====================

::flatbuffers::Offset<fbs::osi3::TrafficSignValue> ConvertTrafficSignValue(const osi3::TrafficSignValue&   value,
                                                                           flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::TrafficSignValueBuilder b(builder);

    auto v    = value.has_value() ? value.value() : 0.0;
    auto unit = value.has_value_unit() ? static_cast<fbs::osi3::TrafficSignValue_::Unit>(value.value_unit())
                                       : static_cast<fbs::osi3::TrafficSignValue_::Unit>(0);
    auto text = value.has_text() ? builder.CreateString(value.text()) : flatbuffers::Offset<flatbuffers::String>();

    return fbs::osi3::CreateTrafficSignValue(builder, v, unit, text);
}

// ===================== TrafficSign::MainSign::Classification =====================

::flatbuffers::Offset<fbs::osi3::TrafficSign_::MainSign_::Classification> ConvertTrafficSignMainSignClassification(
    const osi3::TrafficSign::MainSign::Classification& cls,
    flatbuffers::FlatBufferBuilder&                    builder)
{
    fbs::osi3::TrafficSign_::MainSign_::ClassificationBuilder b(builder);

    auto variability = cls.has_variability() ? static_cast<fbs::osi3::TrafficSign_::Variability>(cls.variability())
                                             : static_cast<fbs::osi3::TrafficSign_::Variability>(0);

    auto type = cls.has_type() ? static_cast<fbs::osi3::TrafficSign_::MainSign_::Classification_::Type>(cls.type())
                               : static_cast<fbs::osi3::TrafficSign_::MainSign_::Classification_::Type>(0);

    auto value = cls.has_value() ? ConvertTrafficSignValue(cls.value(), builder) : flatbuffers::Offset<fbs::osi3::TrafficSignValue>();

    auto direction_scope = cls.has_direction_scope()
                               ? static_cast<fbs::osi3::TrafficSign_::MainSign_::Classification_::DirectionScope>(cls.direction_scope())
                               : static_cast<fbs::osi3::TrafficSign_::MainSign_::Classification_::DirectionScope>(0);

    std::vector<flatbuffers::Offset<fbs::osi3::Identifier>> assigned_lane_ids;
    for (const auto& id : cls.assigned_lane_id())
    {
        assigned_lane_ids.push_back(ConvertIdentifier(id, builder));
    }
    auto assigned_lane_id = builder.CreateVector(assigned_lane_ids);

    auto vertically_mirrored = cls.has_vertically_mirrored() ? cls.vertically_mirrored() : false;
    auto is_out_of_service   = cls.has_is_out_of_service() ? cls.is_out_of_service() : false;

    auto country = cls.has_country() ? builder.CreateString(cls.country()) : flatbuffers::Offset<flatbuffers::String>();

    auto country_revision = cls.has_country_revision() ? builder.CreateString(cls.country_revision()) : flatbuffers::Offset<flatbuffers::String>();

    auto code = cls.has_code() ? builder.CreateString(cls.code()) : flatbuffers::Offset<flatbuffers::String>();

    auto sub_code = cls.has_sub_code() ? builder.CreateString(cls.sub_code()) : flatbuffers::Offset<flatbuffers::String>();

    std::vector<flatbuffers::Offset<fbs::osi3::LogicalLaneAssignment>> logical_lane_assignments;
    for (const auto& lla : cls.logical_lane_assignment())
    {
        logical_lane_assignments.push_back(ConvertLogicalLaneAssignment(lla, builder));
    }
    auto logical_lane_assignment = builder.CreateVector(logical_lane_assignments);

    return fbs::osi3::TrafficSign_::MainSign_::CreateClassification(builder,
                                                                    variability,
                                                                    type,
                                                                    value,
                                                                    direction_scope,
                                                                    assigned_lane_id,
                                                                    vertically_mirrored,
                                                                    is_out_of_service,
                                                                    country,
                                                                    country_revision,
                                                                    code,
                                                                    sub_code,
                                                                    logical_lane_assignment);
}

// ===================== TrafficSign::MainSign =====================

::flatbuffers::Offset<fbs::osi3::TrafficSign_::MainSign> ConvertTrafficSignMainSign(const osi3::TrafficSign::MainSign& main_sign,
                                                                                    flatbuffers::FlatBufferBuilder&    builder)
{
    fbs::osi3::TrafficSign_::MainSignBuilder b(builder);

    auto base = main_sign.has_base() ? ConvertBaseStationary(main_sign.base(), builder) : flatbuffers::Offset<fbs::osi3::BaseStationary>();

    auto classification = main_sign.has_classification() ? ConvertTrafficSignMainSignClassification(main_sign.classification(), builder)
                                                         : flatbuffers::Offset<fbs::osi3::TrafficSign_::MainSign_::Classification>();

    auto model_reference =
        main_sign.has_model_reference() ? builder.CreateString(main_sign.model_reference()) : flatbuffers::Offset<flatbuffers::String>();

    return fbs::osi3::TrafficSign_::CreateMainSign(builder, base, classification, model_reference);
}

// ===================== SupplementarySign::Classification::Arrow =====================

::flatbuffers::Offset<fbs::osi3::TrafficSign_::SupplementarySign_::Classification_::Arrow> ConvertTrafficSignSupplementaryArrow(
    const osi3::TrafficSign::SupplementarySign::Classification::Arrow& arrow,
    flatbuffers::FlatBufferBuilder&                                    builder)
{
    fbs::osi3::TrafficSign_::SupplementarySign_::Classification_::ArrowBuilder b(builder);

    std::vector<flatbuffers::Offset<fbs::osi3::Identifier>> lane_ids;
    for (const auto& id : arrow.lane_id())
    {
        lane_ids.push_back(ConvertIdentifier(id, builder));
    }
    auto lane_id = builder.CreateVector(lane_ids);

    std::vector<int32_t> directions;
    for (const auto& dir : arrow.direction())
    {
        directions.push_back(static_cast<int32_t>(dir));
    }
    auto direction = builder.CreateVector(directions);

    return fbs::osi3::TrafficSign_::SupplementarySign_::Classification_::CreateArrow(builder, lane_id, direction);
}

// ===================== SupplementarySign::Classification =====================

::flatbuffers::Offset<fbs::osi3::TrafficSign_::SupplementarySign_::Classification> ConvertTrafficSignSupplementaryClassification(
    const osi3::TrafficSign::SupplementarySign::Classification& cls,
    flatbuffers::FlatBufferBuilder&                             builder)
{
    fbs::osi3::TrafficSign_::SupplementarySign_::ClassificationBuilder b(builder);

    auto variability = cls.has_variability() ? static_cast<fbs::osi3::TrafficSign_::Variability>(cls.variability())
                                             : static_cast<fbs::osi3::TrafficSign_::Variability>(0);

    auto type = cls.has_type() ? static_cast<fbs::osi3::TrafficSign_::SupplementarySign_::Classification_::Type>(cls.type())
                               : static_cast<fbs::osi3::TrafficSign_::SupplementarySign_::Classification_::Type>(0);

    std::vector<flatbuffers::Offset<fbs::osi3::TrafficSignValue>> values;
    for (const auto& v : cls.value())
    {
        values.push_back(ConvertTrafficSignValue(v, builder));
    }
    auto value = builder.CreateVector(values);

    std::vector<flatbuffers::Offset<fbs::osi3::Identifier>> assigned_lane_ids;
    for (const auto& id : cls.assigned_lane_id())
    {
        assigned_lane_ids.push_back(ConvertIdentifier(id, builder));
    }
    auto assigned_lane_id = builder.CreateVector(assigned_lane_ids);

    std::vector<int32_t> actors;
    for (const auto& a : cls.actor())
    {
        actors.push_back(static_cast<int32_t>(a));
    }
    auto actor = builder.CreateVector(actors);

    std::vector<flatbuffers::Offset<fbs::osi3::TrafficSign_::SupplementarySign_::Classification_::Arrow>> arrows;
    for (const auto& ar : cls.arrow())
    {
        arrows.push_back(ConvertTrafficSignSupplementaryArrow(ar, builder));
    }
    auto arrow = builder.CreateVector(arrows);

    auto is_out_of_service = cls.has_is_out_of_service() ? cls.is_out_of_service() : false;

    auto country = cls.has_country() ? builder.CreateString(cls.country()) : flatbuffers::Offset<flatbuffers::String>();

    auto country_revision = cls.has_country_revision() ? builder.CreateString(cls.country_revision()) : flatbuffers::Offset<flatbuffers::String>();

    auto code = cls.has_code() ? builder.CreateString(cls.code()) : flatbuffers::Offset<flatbuffers::String>();

    auto sub_code = cls.has_sub_code() ? builder.CreateString(cls.sub_code()) : flatbuffers::Offset<flatbuffers::String>();

    std::vector<flatbuffers::Offset<fbs::osi3::LogicalLaneAssignment>> logical_lane_assignments;
    for (const auto& lla : cls.logical_lane_assignment())
    {
        logical_lane_assignments.push_back(ConvertLogicalLaneAssignment(lla, builder));
    }
    auto logical_lane_assignment = builder.CreateVector(logical_lane_assignments);

    return fbs::osi3::TrafficSign_::SupplementarySign_::CreateClassification(builder,
                                                                             variability,
                                                                             type,
                                                                             value,
                                                                             assigned_lane_id,
                                                                             actor,
                                                                             arrow,
                                                                             is_out_of_service,
                                                                             country,
                                                                             country_revision,
                                                                             code,
                                                                             sub_code,
                                                                             logical_lane_assignment);
}

// ===================== SupplementarySign =====================

::flatbuffers::Offset<fbs::osi3::TrafficSign_::SupplementarySign> ConvertTrafficSignSupplementarySign(
    const osi3::TrafficSign::SupplementarySign& sign,
    flatbuffers::FlatBufferBuilder&             builder)
{
    fbs::osi3::TrafficSign_::SupplementarySignBuilder b(builder);

    auto base = sign.has_base() ? ConvertBaseStationary(sign.base(), builder) : flatbuffers::Offset<fbs::osi3::BaseStationary>();

    auto classification = sign.has_classification() ? ConvertTrafficSignSupplementaryClassification(sign.classification(), builder)
                                                    : flatbuffers::Offset<fbs::osi3::TrafficSign_::SupplementarySign_::Classification>();

    auto model_reference = sign.has_model_reference() ? builder.CreateString(sign.model_reference()) : flatbuffers::Offset<flatbuffers::String>();

    return fbs::osi3::TrafficSign_::CreateSupplementarySign(builder, base, classification, model_reference);
}

// ===================== TrafficSign =====================

::flatbuffers::Offset<fbs::osi3::TrafficSign> ConvertTrafficSign(const osi3::TrafficSign& sign, flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::TrafficSignBuilder b(builder);

    auto id = sign.has_id() ? ConvertIdentifier(sign.id(), builder) : flatbuffers::Offset<fbs::osi3::Identifier>();

    auto main_sign =
        sign.has_main_sign() ? ConvertTrafficSignMainSign(sign.main_sign(), builder) : flatbuffers::Offset<fbs::osi3::TrafficSign_::MainSign>();

    std::vector<flatbuffers::Offset<fbs::osi3::TrafficSign_::SupplementarySign>> supplementary_signs;
    for (const auto& s : sign.supplementary_sign())
    {
        supplementary_signs.push_back(ConvertTrafficSignSupplementarySign(s, builder));
    }
    auto supplementary_sign = builder.CreateVector(supplementary_signs);

    std::vector<flatbuffers::Offset<fbs::osi3::ExternalReference>> source_refs;
    for (const auto& ref : sign.source_reference())
    {
        source_refs.push_back(ConvertExternalReference(ref, builder));
    }
    auto source_reference = builder.CreateVector(source_refs);

    return fbs::osi3::CreateTrafficSign(builder, id, main_sign, supplementary_sign, source_reference);
}

// osi_roadmarking

// ==========================
// RoadMarking - Classification
// ==========================

::flatbuffers::Offset<fbs::osi3::RoadMarking_::Classification> ConvertRoadMarkingClassification(const osi3::RoadMarking::Classification& proto,
                                                                                                flatbuffers::FlatBufferBuilder&          builder)
{
    fbs::osi3::RoadMarking_::ClassificationBuilder b(builder);

    auto type = proto.has_type() ? static_cast<fbs::osi3::RoadMarking_::Classification_::Type>(proto.type())
                                 : static_cast<fbs::osi3::RoadMarking_::Classification_::Type>(0);

    auto traffic_main_sign_type = proto.has_traffic_main_sign_type()
                                      ? static_cast<fbs::osi3::TrafficSign_::MainSign_::Classification_::Type>(proto.traffic_main_sign_type())
                                      : static_cast<fbs::osi3::TrafficSign_::MainSign_::Classification_::Type>(0);

    auto monochrome_color = proto.has_monochrome_color() ? static_cast<fbs::osi3::RoadMarking_::Classification_::Color>(proto.monochrome_color())
                                                         : static_cast<fbs::osi3::RoadMarking_::Classification_::Color>(0);

    auto value = proto.has_value() ? ConvertTrafficSignValue(proto.value(), builder) : 0;

    auto value_text = proto.has_value_text() ? builder.CreateString(proto.value_text()) : 0;

    std::vector<::flatbuffers::Offset<fbs::osi3::Identifier>> assigned_lane_id;
    for (const auto& id : proto.assigned_lane_id())
    {
        assigned_lane_id.push_back(ConvertIdentifier(id, builder));
    }

    auto is_out_of_service = proto.has_is_out_of_service() ? proto.is_out_of_service() : false;

    auto country = proto.has_country() ? builder.CreateString(proto.country()) : 0;

    auto country_revision = proto.has_country_revision() ? builder.CreateString(proto.country_revision()) : 0;

    auto code = proto.has_code() ? builder.CreateString(proto.code()) : 0;

    auto sub_code = proto.has_sub_code() ? builder.CreateString(proto.sub_code()) : 0;

    std::vector<::flatbuffers::Offset<fbs::osi3::LogicalLaneAssignment>> logical_lane_assignment;
    for (const auto& l : proto.logical_lane_assignment())
    {
        logical_lane_assignment.push_back(ConvertLogicalLaneAssignment(l, builder));
    }

    return fbs::osi3::RoadMarking_::CreateClassification(builder,
                                                         type,
                                                         traffic_main_sign_type,
                                                         monochrome_color,
                                                         value,
                                                         value_text,
                                                         builder.CreateVector(assigned_lane_id),
                                                         is_out_of_service,
                                                         country,
                                                         country_revision,
                                                         code,
                                                         sub_code,
                                                         builder.CreateVector(logical_lane_assignment));
}

// ==========================
// RoadMarking
// ==========================

::flatbuffers::Offset<fbs::osi3::RoadMarking> ConvertRoadMarking(const osi3::RoadMarking& proto, flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::RoadMarkingBuilder b(builder);

    auto id = proto.has_id() ? ConvertIdentifier(proto.id(), builder) : 0;

    auto base = proto.has_base() ? ConvertBaseStationary(proto.base(), builder) : 0;

    auto classification = proto.has_classification() ? ConvertRoadMarkingClassification(proto.classification(), builder) : 0;

    std::vector<::flatbuffers::Offset<fbs::osi3::ExternalReference>> source_reference;
    for (const auto& s : proto.source_reference())
    {
        source_reference.push_back(ConvertExternalReference(s, builder));
    }

    auto color_description = proto.has_color_description() ? ConvertColorDescription(proto.color_description(), builder) : 0;

    return fbs::osi3::CreateRoadMarking(builder, id, base, classification, builder.CreateVector(source_reference), color_description);
}

// osi_logical_lane

// ===================== LogicalLaneBoundary::LogicalBoundaryPoint =====================

::flatbuffers::Offset<fbs::osi3::LogicalLaneBoundary_::LogicalBoundaryPoint> ConvertLogicalLaneBoundaryLogicalBoundaryPoint(
    const osi3::LogicalLaneBoundary::LogicalBoundaryPoint& point,
    flatbuffers::FlatBufferBuilder&                        builder)
{
    fbs::osi3::LogicalLaneBoundary_::LogicalBoundaryPointBuilder b(builder);

    auto position = point.has_position() ? ConvertVector3d(point.position(), builder) : flatbuffers::Offset<fbs::osi3::Vector3d>();

    auto s_position = point.has_s_position() ? point.s_position() : 0.0;
    auto t_position = point.has_t_position() ? point.t_position() : 0.0;

    return fbs::osi3::LogicalLaneBoundary_::CreateLogicalBoundaryPoint(builder, position, s_position, t_position);
}

// ===================== LogicalLaneBoundary =====================

::flatbuffers::Offset<fbs::osi3::LogicalLaneBoundary> ConvertLogicalLaneBoundary(const osi3::LogicalLaneBoundary& boundary,
                                                                                 flatbuffers::FlatBufferBuilder&  builder)
{
    fbs::osi3::LogicalLaneBoundaryBuilder b(builder);

    auto id = boundary.has_id() ? ConvertIdentifier(boundary.id(), builder) : flatbuffers::Offset<fbs::osi3::Identifier>();

    std::vector<flatbuffers::Offset<fbs::osi3::LogicalLaneBoundary_::LogicalBoundaryPoint>> boundary_points;
    for (const auto& p : boundary.boundary_line())
    {
        boundary_points.push_back(ConvertLogicalLaneBoundaryLogicalBoundaryPoint(p, builder));
    }
    auto boundary_line = builder.CreateVector(boundary_points);

    auto reference_line_id =
        boundary.has_reference_line_id() ? ConvertIdentifier(boundary.reference_line_id(), builder) : flatbuffers::Offset<fbs::osi3::Identifier>();

    std::vector<flatbuffers::Offset<fbs::osi3::Identifier>> physical_boundary_ids;
    for (const auto& idp : boundary.physical_boundary_id())
    {
        physical_boundary_ids.push_back(ConvertIdentifier(idp, builder));
    }
    auto physical_boundary_id = builder.CreateVector(physical_boundary_ids);

    auto passing_rule = boundary.has_passing_rule() ? static_cast<fbs::osi3::LogicalLaneBoundary_::PassingRule>(boundary.passing_rule())
                                                    : static_cast<fbs::osi3::LogicalLaneBoundary_::PassingRule>(0);

    std::vector<flatbuffers::Offset<fbs::osi3::ExternalReference>> source_refs;
    for (const auto& ref : boundary.source_reference())
    {
        source_refs.push_back(ConvertExternalReference(ref, builder));
    }
    auto source_reference = builder.CreateVector(source_refs);

    return fbs::osi3::CreateLogicalLaneBoundary(builder, id, boundary_line, reference_line_id, physical_boundary_id, passing_rule, source_reference);
}

// ===================== LogicalLane::PhysicalLaneReference =====================

::flatbuffers::Offset<fbs::osi3::LogicalLane_::PhysicalLaneReference> ConvertLogicalLanePhysicalLaneReference(
    const osi3::LogicalLane::PhysicalLaneReference& ref,
    flatbuffers::FlatBufferBuilder&                 builder)
{
    fbs::osi3::LogicalLane_::PhysicalLaneReferenceBuilder b(builder);

    auto physical_lane_id =
        ref.has_physical_lane_id() ? ConvertIdentifier(ref.physical_lane_id(), builder) : flatbuffers::Offset<fbs::osi3::Identifier>();

    auto start_s = ref.has_start_s() ? ref.start_s() : 0.0;
    auto end_s   = ref.has_end_s() ? ref.end_s() : 0.0;

    return fbs::osi3::LogicalLane_::CreatePhysicalLaneReference(builder, physical_lane_id, start_s, end_s);
}

// ===================== LogicalLane::LaneConnection =====================

::flatbuffers::Offset<fbs::osi3::LogicalLane_::LaneConnection> ConvertLogicalLaneLaneConnection(const osi3::LogicalLane::LaneConnection& conn,
                                                                                                flatbuffers::FlatBufferBuilder&          builder)
{
    fbs::osi3::LogicalLane_::LaneConnectionBuilder b(builder);

    auto other_lane_id = conn.has_other_lane_id() ? ConvertIdentifier(conn.other_lane_id(), builder) : flatbuffers::Offset<fbs::osi3::Identifier>();

    auto at_begin = conn.has_at_begin_of_other_lane() ? conn.at_begin_of_other_lane() : false;

    return fbs::osi3::LogicalLane_::CreateLaneConnection(builder, other_lane_id, at_begin);
}

// ===================== LogicalLane::LaneRelation =====================

::flatbuffers::Offset<fbs::osi3::LogicalLane_::LaneRelation> ConvertLogicalLaneLaneRelation(const osi3::LogicalLane::LaneRelation& rel,
                                                                                            flatbuffers::FlatBufferBuilder&        builder)
{
    fbs::osi3::LogicalLane_::LaneRelationBuilder b(builder);

    auto other_lane_id = rel.has_other_lane_id() ? ConvertIdentifier(rel.other_lane_id(), builder) : flatbuffers::Offset<fbs::osi3::Identifier>();

    auto start_s       = rel.has_start_s() ? rel.start_s() : 0.0;
    auto end_s         = rel.has_end_s() ? rel.end_s() : 0.0;
    auto start_s_other = rel.has_start_s_other() ? rel.start_s_other() : 0.0;
    auto end_s_other   = rel.has_end_s_other() ? rel.end_s_other() : 0.0;

    return fbs::osi3::LogicalLane_::CreateLaneRelation(builder, other_lane_id, start_s, end_s, start_s_other, end_s_other);
}

// ===================== TrafficRule::TrafficRuleValidity::TypeValidity =====================

::flatbuffers::Offset<fbs::osi3::LogicalLane_::TrafficRule_::TrafficRuleValidity_::TypeValidity> ConvertLogicalLaneTrafficRuleTypeValidity(
    const osi3::LogicalLane::TrafficRule::TrafficRuleValidity::TypeValidity& tv,
    flatbuffers::FlatBufferBuilder&                                          builder)
{
    fbs::osi3::LogicalLane_::TrafficRule_::TrafficRuleValidity_::TypeValidityBuilder b(builder);

    auto type = tv.has_type() ? static_cast<fbs::osi3::MovingObject_::Type>(tv.type()) : static_cast<fbs::osi3::MovingObject_::Type>(0);

    auto vehicle_type = tv.has_vehicle_type() ? static_cast<fbs::osi3::MovingObject_::VehicleClassification_::Type>(tv.vehicle_type())
                                              : static_cast<fbs::osi3::MovingObject_::VehicleClassification_::Type>(0);

    auto vehicle_role = tv.has_vehicle_role() ? static_cast<fbs::osi3::MovingObject_::VehicleClassification_::Role>(tv.vehicle_role())
                                              : static_cast<fbs::osi3::MovingObject_::VehicleClassification_::Role>(0);

    return fbs::osi3::LogicalLane_::TrafficRule_::TrafficRuleValidity_::CreateTypeValidity(builder, type, vehicle_type, vehicle_role);
}

// ===================== TrafficRule::TrafficRuleValidity =====================

::flatbuffers::Offset<fbs::osi3::LogicalLane_::TrafficRule_::TrafficRuleValidity> ConvertLogicalLaneTrafficRuleValidity(
    const osi3::LogicalLane::TrafficRule::TrafficRuleValidity& validity,
    flatbuffers::FlatBufferBuilder&                            builder)
{
    fbs::osi3::LogicalLane_::TrafficRule_::TrafficRuleValidityBuilder b(builder);

    auto start_s = validity.has_start_s() ? validity.start_s() : 0.0;
    auto end_s   = validity.has_end_s() ? validity.end_s() : 0.0;

    std::vector<flatbuffers::Offset<fbs::osi3::LogicalLane_::TrafficRule_::TrafficRuleValidity_::TypeValidity>> valid_for;
    for (const auto& v : validity.valid_for_type())
    {
        valid_for.push_back(ConvertLogicalLaneTrafficRuleTypeValidity(v, builder));
    }
    auto valid_for_type = builder.CreateVector(valid_for);

    return fbs::osi3::LogicalLane_::TrafficRule_::CreateTrafficRuleValidity(builder, start_s, end_s, valid_for_type);
}

// ===================== TrafficRule::SpeedLimit =====================

::flatbuffers::Offset<fbs::osi3::LogicalLane_::TrafficRule_::SpeedLimit> ConvertLogicalLaneTrafficRuleSpeedLimit(
    const osi3::LogicalLane::TrafficRule::SpeedLimit& sl,
    flatbuffers::FlatBufferBuilder&                   builder)
{
    fbs::osi3::LogicalLane_::TrafficRule_::SpeedLimitBuilder b(builder);

    auto speed_limit_value =
        sl.has_speed_limit_value() ? ConvertTrafficSignValue(sl.speed_limit_value(), builder) : flatbuffers::Offset<fbs::osi3::TrafficSignValue>();

    return fbs::osi3::LogicalLane_::TrafficRule_::CreateSpeedLimit(builder, speed_limit_value);
}

// ===================== TrafficRule =====================

::flatbuffers::Offset<fbs::osi3::LogicalLane_::TrafficRule> ConvertLogicalLaneTrafficRule(const osi3::LogicalLane::TrafficRule& rule,
                                                                                          flatbuffers::FlatBufferBuilder&       builder)
{
    fbs::osi3::LogicalLane_::TrafficRuleBuilder b(builder);

    auto traffic_rule_type = rule.has_traffic_rule_type()
                                 ? static_cast<fbs::osi3::LogicalLane_::TrafficRule_::TrafficRuleType>(rule.traffic_rule_type())
                                 : static_cast<fbs::osi3::LogicalLane_::TrafficRule_::TrafficRuleType>(0);

    auto traffic_rule_validity = rule.has_traffic_rule_validity() ? ConvertLogicalLaneTrafficRuleValidity(rule.traffic_rule_validity(), builder)
                                                                  : flatbuffers::Offset<fbs::osi3::LogicalLane_::TrafficRule_::TrafficRuleValidity>();

    auto speed_limit = rule.has_speed_limit() ? ConvertLogicalLaneTrafficRuleSpeedLimit(rule.speed_limit(), builder)
                                              : flatbuffers::Offset<fbs::osi3::LogicalLane_::TrafficRule_::SpeedLimit>();

    return fbs::osi3::LogicalLane_::CreateTrafficRule(builder, traffic_rule_type, traffic_rule_validity, speed_limit);
}

// ===================== LogicalLane =====================

::flatbuffers::Offset<fbs::osi3::LogicalLane> ConvertLogicalLane(const osi3::LogicalLane& lane, flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::LogicalLaneBuilder b(builder);

    auto id = lane.has_id() ? ConvertIdentifier(lane.id(), builder) : flatbuffers::Offset<fbs::osi3::Identifier>();

    auto type = lane.has_type() ? static_cast<fbs::osi3::LogicalLane_::Type>(lane.type()) : static_cast<fbs::osi3::LogicalLane_::Type>(0);

    std::vector<flatbuffers::Offset<fbs::osi3::ExternalReference>> source_refs;
    for (const auto& ref : lane.source_reference())
    {
        source_refs.push_back(ConvertExternalReference(ref, builder));
    }
    auto source_reference = builder.CreateVector(source_refs);

    std::vector<flatbuffers::Offset<fbs::osi3::LogicalLane_::PhysicalLaneReference>> physical_refs;
    for (const auto& pr : lane.physical_lane_reference())
    {
        physical_refs.push_back(ConvertLogicalLanePhysicalLaneReference(pr, builder));
    }
    auto physical_lane_reference = builder.CreateVector(physical_refs);

    auto reference_line_id =
        lane.has_reference_line_id() ? ConvertIdentifier(lane.reference_line_id(), builder) : flatbuffers::Offset<fbs::osi3::Identifier>();

    auto start_s = lane.has_start_s() ? lane.start_s() : 0.0;
    auto end_s   = lane.has_end_s() ? lane.end_s() : 0.0;

    auto move_direction = lane.has_move_direction() ? static_cast<fbs::osi3::LogicalLane_::MoveDirection>(lane.move_direction())
                                                    : static_cast<fbs::osi3::LogicalLane_::MoveDirection>(0);

    std::vector<flatbuffers::Offset<fbs::osi3::LogicalLane_::LaneRelation>> right_adj;
    for (const auto& r : lane.right_adjacent_lane())
        right_adj.push_back(ConvertLogicalLaneLaneRelation(r, builder));
    auto right_adjacent_lane = builder.CreateVector(right_adj);

    std::vector<flatbuffers::Offset<fbs::osi3::LogicalLane_::LaneRelation>> left_adj;
    for (const auto& l : lane.left_adjacent_lane())
        left_adj.push_back(ConvertLogicalLaneLaneRelation(l, builder));
    auto left_adjacent_lane = builder.CreateVector(left_adj);

    std::vector<flatbuffers::Offset<fbs::osi3::LogicalLane_::LaneRelation>> overlapping;
    for (const auto& o : lane.overlapping_lane())
        overlapping.push_back(ConvertLogicalLaneLaneRelation(o, builder));
    auto overlapping_lane = builder.CreateVector(overlapping);

    std::vector<flatbuffers::Offset<fbs::osi3::Identifier>> right_boundary_ids;
    for (const auto& idb : lane.right_boundary_id())
        right_boundary_ids.push_back(ConvertIdentifier(idb, builder));
    auto right_boundary_id = builder.CreateVector(right_boundary_ids);

    std::vector<flatbuffers::Offset<fbs::osi3::Identifier>> left_boundary_ids;
    for (const auto& idb : lane.left_boundary_id())
        left_boundary_ids.push_back(ConvertIdentifier(idb, builder));
    auto left_boundary_id = builder.CreateVector(left_boundary_ids);

    std::vector<flatbuffers::Offset<fbs::osi3::LogicalLane_::LaneConnection>> predecessors;
    for (const auto& p : lane.predecessor_lane())
        predecessors.push_back(ConvertLogicalLaneLaneConnection(p, builder));
    auto predecessor_lane = builder.CreateVector(predecessors);

    std::vector<flatbuffers::Offset<fbs::osi3::LogicalLane_::LaneConnection>> successors;
    for (const auto& s : lane.successor_lane())
        successors.push_back(ConvertLogicalLaneLaneConnection(s, builder));
    auto successor_lane = builder.CreateVector(successors);

    auto street_name = lane.has_street_name() ? builder.CreateString(lane.street_name()) : flatbuffers::Offset<flatbuffers::String>();

    std::vector<flatbuffers::Offset<fbs::osi3::LogicalLane_::TrafficRule>> traffic_rules;
    for (const auto& tr : lane.traffic_rule())
        traffic_rules.push_back(ConvertLogicalLaneTrafficRule(tr, builder));
    auto traffic_rule = builder.CreateVector(traffic_rules);

    return fbs::osi3::CreateLogicalLane(builder,
                                        id,
                                        type,
                                        source_reference,
                                        physical_lane_reference,
                                        reference_line_id,
                                        start_s,
                                        end_s,
                                        move_direction,
                                        right_adjacent_lane,
                                        left_adjacent_lane,
                                        overlapping_lane,
                                        right_boundary_id,
                                        left_boundary_id,
                                        predecessor_lane,
                                        successor_lane,
                                        street_name,
                                        traffic_rule);
}

// osi_groundtruth

// ===================== GroundTruth::ProjFrameOffset =====================

::flatbuffers::Offset<fbs::osi3::GroundTruth_::ProjFrameOffset> ConvertGroundTruthProjFrameOffset(const osi3::GroundTruth::ProjFrameOffset& offset,
                                                                                                  flatbuffers::FlatBufferBuilder&           builder)
{
    fbs::osi3::GroundTruth_::ProjFrameOffsetBuilder b(builder);

    auto position = offset.has_position() ? ConvertVector3d(offset.position(), builder) : flatbuffers::Offset<fbs::osi3::Vector3d>();

    auto yaw = offset.has_yaw() ? offset.yaw() : 0.0;

    return fbs::osi3::GroundTruth_::CreateProjFrameOffset(builder, position, yaw);
}

// ===================== GroundTruth =====================

::flatbuffers::Offset<fbs::osi3::GroundTruth> ConvertGroundTruth(const osi3::GroundTruth& gt, flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::GroundTruthBuilder b(builder);

    auto timestamp = gt.has_timestamp() ? ConvertTimestamp(gt.timestamp(), builder) : flatbuffers::Offset<fbs::osi3::Timestamp>();

    auto host_vehicle_id = gt.has_host_vehicle_id() ? ConvertIdentifier(gt.host_vehicle_id(), builder) : flatbuffers::Offset<fbs::osi3::Identifier>();

    std::vector<flatbuffers::Offset<fbs::osi3::StationaryObject>> stationary_objects;
    for (const auto& obj : gt.stationary_object())
    {
        stationary_objects.push_back(ConvertStationaryObject(obj, builder));
    }
    auto stationary_object = builder.CreateVector(stationary_objects);

    std::vector<flatbuffers::Offset<fbs::osi3::MovingObject>> moving_objects;
    for (const auto& obj : gt.moving_object())
    {
        moving_objects.push_back(ConvertMovingObject(obj, builder));
    }
    auto moving_object = builder.CreateVector(moving_objects);

    std::vector<flatbuffers::Offset<fbs::osi3::TrafficSign>> traffic_signs;
    for (const auto& ts : gt.traffic_sign())
    {
        traffic_signs.push_back(ConvertTrafficSign(ts, builder));
    }
    auto traffic_sign = builder.CreateVector(traffic_signs);

    std::vector<flatbuffers::Offset<fbs::osi3::TrafficLight>> traffic_lights;
    for (const auto& tl : gt.traffic_light())
    {
        traffic_lights.push_back(ConvertTrafficLight(tl, builder));
    }
    auto traffic_light = builder.CreateVector(traffic_lights);

    std::vector<flatbuffers::Offset<fbs::osi3::RoadMarking>> road_markings;
    for (const auto& rm : gt.road_marking())
    {
        road_markings.push_back(ConvertRoadMarking(rm, builder));
    }
    auto road_marking = builder.CreateVector(road_markings);

    std::vector<flatbuffers::Offset<fbs::osi3::LaneBoundary>> lane_boundaries;
    for (const auto& lb : gt.lane_boundary())
    {
        lane_boundaries.push_back(ConvertLaneBoundary(lb, builder));
    }
    auto lane_boundary = builder.CreateVector(lane_boundaries);

    std::vector<flatbuffers::Offset<fbs::osi3::Lane>> lanes;
    for (const auto& l : gt.lane())
    {
        lanes.push_back(ConvertLane(l, builder));
    }
    auto lane = builder.CreateVector(lanes);

    std::vector<flatbuffers::Offset<fbs::osi3::Occupant>> occupants;
    for (const auto& o : gt.occupant())
    {
        occupants.push_back(ConvertOccupant(o, builder));
    }
    auto occupant = builder.CreateVector(occupants);

    auto environmental_conditions = gt.has_environmental_conditions() ? ConvertEnvironmentalConditions(gt.environmental_conditions(), builder)
                                                                      : flatbuffers::Offset<fbs::osi3::EnvironmentalConditions>();

    auto country_code = gt.has_country_code() ? gt.country_code() : 0u;

    auto proj_string = gt.has_proj_string() ? builder.CreateString(gt.proj_string()) : flatbuffers::Offset<flatbuffers::String>();

    auto map_reference = gt.has_map_reference() ? builder.CreateString(gt.map_reference()) : flatbuffers::Offset<flatbuffers::String>();

    auto model_reference = gt.has_model_reference() ? builder.CreateString(gt.model_reference()) : flatbuffers::Offset<flatbuffers::String>();

    std::vector<flatbuffers::Offset<fbs::osi3::ReferenceLine>> reference_lines;
    for (const auto& rl : gt.reference_line())
    {
        reference_lines.push_back(ConvertReferenceLine(rl, builder));
    }
    auto reference_line = builder.CreateVector(reference_lines);

    std::vector<flatbuffers::Offset<fbs::osi3::LogicalLaneBoundary>> logical_lane_boundaries;
    for (const auto& llb : gt.logical_lane_boundary())
    {
        logical_lane_boundaries.push_back(ConvertLogicalLaneBoundary(llb, builder));
    }
    auto logical_lane_boundary = builder.CreateVector(logical_lane_boundaries);

    std::vector<flatbuffers::Offset<fbs::osi3::LogicalLane>> logical_lanes;
    for (const auto& ll : gt.logical_lane())
    {
        logical_lanes.push_back(ConvertLogicalLane(ll, builder));
    }
    auto logical_lane = builder.CreateVector(logical_lanes);

    auto proj_frame_offset = gt.has_proj_frame_offset() ? ConvertGroundTruthProjFrameOffset(gt.proj_frame_offset(), builder)
                                                        : flatbuffers::Offset<fbs::osi3::GroundTruth_::ProjFrameOffset>();

    return fbs::osi3::CreateGroundTruth(builder,
                                        timestamp,
                                        host_vehicle_id,
                                        stationary_object,
                                        moving_object,
                                        traffic_sign,
                                        traffic_light,
                                        road_marking,
                                        lane_boundary,
                                        lane,
                                        occupant,
                                        environmental_conditions,
                                        country_code,
                                        proj_string,
                                        map_reference,
                                        model_reference,
                                        reference_line,
                                        logical_lane_boundary,
                                        logical_lane,
                                        proj_frame_offset);
}
