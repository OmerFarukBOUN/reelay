#include <flatbuffers/flatbuffers.h>
namespace fbs
{
#include "osi_trafficsign_generated.h"
}

#include "osi_trafficsign.pb.h"

#include <vector>

#include "osi_common_converter.hpp"

// ==========================
// TrafficSignValue
// ==========================

::flatbuffers::Offset<fbs::osi3::TrafficSignValue> ConvertTrafficSignValue(const osi3::TrafficSignValue& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::TrafficSignValueBuilder b(builder);
    
    if (proto.has_value())
    	b.add_value(proto.value());
    
    if (proto.has_value_unit())
    	b.add_value_unit(static_cast<fbs::osi3::TrafficSignValue_::Unit>(proto.value_unit()));
    
    if (proto.has_text())
    	b.add_text(builder.CreateString(proto.text()));
    
    return b.Finish();
}

// ==========================
// TrafficSign - MainSign - Classification
// ==========================

::flatbuffers::Offset<fbs::osi3::TrafficSign_::MainSign_::Classification> ConvertTrafficSignMainSignClassification(const osi3::TrafficSign::MainSign::Classification& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::TrafficSign_::MainSign_::ClassificationBuilder b(builder);
    
    if (proto.has_variability())
    	b.add_variability(static_cast<fbs::osi3::TrafficSign_::Variability>(proto.variability()));
    
    if (proto.has_type())
    	b.add_type(static_cast<fbs::osi3::TrafficSign_::MainSign_::Classification_::Type>(proto.type()));
    
    if (proto.has_value())
    	b.add_value(ConvertTrafficSignValue(proto.value(), builder));
    
    if (proto.has_direction_scope())
    	b.add_direction_scope(static_cast<fbs::osi3::TrafficSign_::MainSign_::Classification_::DirectionScope>(proto.direction_scope()));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Identifier>> assigned_lane_id;
    for (const auto& item : proto.assigned_lane_id())
        assigned_lane_id.push_back(ConvertIdentifier(item, builder));
    b.add_assigned_lane_id(builder.CreateVector(assigned_lane_id));
    
    if (proto.has_vertically_mirrored())
    	b.add_vertically_mirrored(proto.vertically_mirrored());
    
    if (proto.has_is_out_of_service())
    	b.add_is_out_of_service(proto.is_out_of_service());
    
    if (proto.has_country())
    	b.add_country(builder.CreateString(proto.country()));
    
    if (proto.has_country_revision())
    	b.add_country_revision(builder.CreateString(proto.country_revision()));
    
    if (proto.has_code())
    	b.add_code(builder.CreateString(proto.code()));
    
    if (proto.has_sub_code())
    	b.add_sub_code(builder.CreateString(proto.sub_code()));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::LogicalLaneAssignment>> logical_lane_assignment;
    for (const auto& item : proto.logical_lane_assignment())
        logical_lane_assignment.push_back(ConvertLogicalLaneAssignment(item, builder));
    b.add_logical_lane_assignment(builder.CreateVector(logical_lane_assignment));
    
    return b.Finish();
}

// ==========================
// TrafficSign - MainSign
// ==========================

::flatbuffers::Offset<fbs::osi3::TrafficSign_::MainSign> ConvertTrafficSignMainSign(const osi3::TrafficSign::MainSign& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::TrafficSign_::MainSignBuilder b(builder);
    
    if (proto.has_base())
    	b.add_base(ConvertBaseStationary(proto.base(), builder));
    
    if (proto.has_classification())
    	b.add_classification(ConvertTrafficSignMainSignClassification(proto.classification(), builder));
    
    if (proto.has_model_reference())
    	b.add_model_reference(builder.CreateString(proto.model_reference()));
    
    return b.Finish();
}

// ==========================
// TrafficSign - SupplementarySign - Classification - Arrow
// ==========================

::flatbuffers::Offset<fbs::osi3::TrafficSign_::SupplementarySign_::Classification_::Arrow> ConvertTrafficSignSupplementarySignClassificationArrow(const osi3::TrafficSign::SupplementarySign::Classification::Arrow& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::TrafficSign_::SupplementarySign_::Classification_::ArrowBuilder b(builder);
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Identifier>> lane_id;
    for (const auto& item : proto.lane_id())
        lane_id.push_back(ConvertIdentifier(item, builder));
    b.add_lane_id(builder.CreateVector(lane_id));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Direction>> direction;
    for (const auto& item : proto.direction())
        direction.push_back(ConvertDirection(item, builder));
    b.add_direction(builder.CreateVector(direction));
    
    return b.Finish();
}

// ==========================
// TrafficSign - SupplementarySign - Classification
// ==========================

::flatbuffers::Offset<fbs::osi3::TrafficSign_::SupplementarySign_::Classification> ConvertTrafficSignSupplementarySignClassification(const osi3::TrafficSign::SupplementarySign::Classification& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::TrafficSign_::SupplementarySign_::ClassificationBuilder b(builder);
    
    if (proto.has_variability())
    	b.add_variability(static_cast<fbs::osi3::TrafficSign_::Variability>(proto.variability()));
    
    if (proto.has_type())
    	b.add_type(static_cast<fbs::osi3::TrafficSign_::SupplementarySign_::Classification_::Type>(proto.type()));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::TrafficSignValue>> value;
    for (const auto& item : proto.value())
        value.push_back(ConvertTrafficSignValue(item, builder));
    b.add_value(builder.CreateVector(value));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Identifier>> assigned_lane_id;
    for (const auto& item : proto.assigned_lane_id())
        assigned_lane_id.push_back(ConvertIdentifier(item, builder));
    b.add_assigned_lane_id(builder.CreateVector(assigned_lane_id));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Actor>> actor;
    for (const auto& item : proto.actor())
        actor.push_back(ConvertActor(item, builder));
    b.add_actor(builder.CreateVector(actor));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::TrafficSign_::SupplementarySign_::Classification_::Arrow>> arrow;
    for (const auto& item : proto.arrow())
        arrow.push_back(ConvertTrafficSignSupplementarySignClassificationArrow(item, builder));
    b.add_arrow(builder.CreateVector(arrow));
    
    if (proto.has_is_out_of_service())
    	b.add_is_out_of_service(proto.is_out_of_service());
    
    if (proto.has_country())
    	b.add_country(builder.CreateString(proto.country()));
    
    if (proto.has_country_revision())
    	b.add_country_revision(builder.CreateString(proto.country_revision()));
    
    if (proto.has_code())
    	b.add_code(builder.CreateString(proto.code()));
    
    if (proto.has_sub_code())
    	b.add_sub_code(builder.CreateString(proto.sub_code()));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::LogicalLaneAssignment>> logical_lane_assignment;
    for (const auto& item : proto.logical_lane_assignment())
        logical_lane_assignment.push_back(ConvertLogicalLaneAssignment(item, builder));
    b.add_logical_lane_assignment(builder.CreateVector(logical_lane_assignment));
    
    return b.Finish();
}

// ==========================
// TrafficSign - SupplementarySign
// ==========================

::flatbuffers::Offset<fbs::osi3::TrafficSign_::SupplementarySign> ConvertTrafficSignSupplementarySign(const osi3::TrafficSign::SupplementarySign& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::TrafficSign_::SupplementarySignBuilder b(builder);
    
    if (proto.has_base())
    	b.add_base(ConvertBaseStationary(proto.base(), builder));
    
    if (proto.has_classification())
    	b.add_classification(ConvertTrafficSignSupplementarySignClassification(proto.classification(), builder));
    
    if (proto.has_model_reference())
    	b.add_model_reference(builder.CreateString(proto.model_reference()));
    
    return b.Finish();
}

// ==========================
// TrafficSign
// ==========================

::flatbuffers::Offset<fbs::osi3::TrafficSign> ConvertTrafficSign(const osi3::TrafficSign& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::TrafficSignBuilder b(builder);
    
    if (proto.has_id())
    	b.add_id(ConvertIdentifier(proto.id(), builder));
    
    if (proto.has_main_sign())
    	b.add_main_sign(ConvertTrafficSignMainSign(proto.main_sign(), builder));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::TrafficSign_::SupplementarySign>> supplementary_sign;
    for (const auto& item : proto.supplementary_sign())
        supplementary_sign.push_back(ConvertTrafficSignSupplementarySign(item, builder));
    b.add_supplementary_sign(builder.CreateVector(supplementary_sign));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::ExternalReference>> source_reference;
    for (const auto& item : proto.source_reference())
        source_reference.push_back(ConvertExternalReference(item, builder));
    b.add_source_reference(builder.CreateVector(source_reference));
    
    return b.Finish();
}
