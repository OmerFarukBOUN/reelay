#include <flatbuffers/flatbuffers.h>
namespace fbs
{
#include "osi_environment_generated.h"
}

#include "osi_environment.pb.h"

#include <vector>

#include "osi_common_converter.hpp"

// ==========================
// EnvironmentalConditions - TimeOfDay
// ==========================

::flatbuffers::Offset<fbs::osi3::EnvironmentalConditions_::TimeOfDay> ConvertEnvironmentalConditionsTimeOfDay(const osi3::EnvironmentalConditions::TimeOfDay& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::EnvironmentalConditions_::TimeOfDayBuilder b(builder);
    
    if (proto.has_seconds_since_midnight())
    	b.add_seconds_since_midnight(proto.seconds_since_midnight());
    
    return b.Finish();
}

// ==========================
// EnvironmentalConditions - CloudLayer
// ==========================

::flatbuffers::Offset<fbs::osi3::EnvironmentalConditions_::CloudLayer> ConvertEnvironmentalConditionsCloudLayer(const osi3::EnvironmentalConditions::CloudLayer& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::EnvironmentalConditions_::CloudLayerBuilder b(builder);
    
    if (proto.has_fractional_cloud_cover())
    	b.add_fractional_cloud_cover(static_cast<fbs::osi3::EnvironmentalConditions_::CloudLayer_::FractionalCloudCover>(proto.fractional_cloud_cover()));
    
    return b.Finish();
}

// ==========================
// EnvironmentalConditions - Wind
// ==========================

::flatbuffers::Offset<fbs::osi3::EnvironmentalConditions_::Wind> ConvertEnvironmentalConditionsWind(const osi3::EnvironmentalConditions::Wind& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::EnvironmentalConditions_::WindBuilder b(builder);
    
    if (proto.has_origin_direction())
    	b.add_origin_direction(proto.origin_direction());
    
    if (proto.has_speed())
    	b.add_speed(proto.speed());
    
    return b.Finish();
}

// ==========================
// EnvironmentalConditions - Sun
// ==========================

::flatbuffers::Offset<fbs::osi3::EnvironmentalConditions_::Sun> ConvertEnvironmentalConditionsSun(const osi3::EnvironmentalConditions::Sun& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::EnvironmentalConditions_::SunBuilder b(builder);
    
    if (proto.has_azimuth())
    	b.add_azimuth(proto.azimuth());
    
    if (proto.has_elevation())
    	b.add_elevation(proto.elevation());
    
    if (proto.has_intensity())
    	b.add_intensity(proto.intensity());
    
    return b.Finish();
}

// ==========================
// EnvironmentalConditions
// ==========================

::flatbuffers::Offset<fbs::osi3::EnvironmentalConditions> ConvertEnvironmentalConditions(const osi3::EnvironmentalConditions& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::EnvironmentalConditionsBuilder b(builder);
    
    if (proto.has_ambient_illumination())
    	b.add_ambient_illumination(static_cast<fbs::osi3::EnvironmentalConditions_::AmbientIllumination>(proto.ambient_illumination()));
    
    if (proto.has_time_of_day())
    	b.add_time_of_day(ConvertEnvironmentalConditionsTimeOfDay(proto.time_of_day(), builder));
    
    if (proto.has_unix_timestamp())
    	b.add_unix_timestamp(proto.unix_timestamp());
    
    if (proto.has_atmospheric_pressure())
    	b.add_atmospheric_pressure(proto.atmospheric_pressure());
    
    if (proto.has_temperature())
    	b.add_temperature(proto.temperature());
    
    if (proto.has_relative_humidity())
    	b.add_relative_humidity(proto.relative_humidity());
    
    if (proto.has_precipitation())
    	b.add_precipitation(static_cast<fbs::osi3::EnvironmentalConditions_::Precipitation>(proto.precipitation()));
    
    if (proto.has_fog())
    	b.add_fog(static_cast<fbs::osi3::EnvironmentalConditions_::Fog>(proto.fog()));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::ExternalReference>> source_reference;
    for (const auto& item : proto.source_reference())
        source_reference.push_back(ConvertExternalReference(item, builder));
    b.add_source_reference(builder.CreateVector(source_reference));
    
    if (proto.has_clouds())
    	b.add_clouds(ConvertEnvironmentalConditionsCloudLayer(proto.clouds(), builder));
    
    if (proto.has_wind())
    	b.add_wind(ConvertEnvironmentalConditionsWind(proto.wind(), builder));
    
    if (proto.has_sun())
    	b.add_sun(ConvertEnvironmentalConditionsSun(proto.sun(), builder));
    
    return b.Finish();
}
