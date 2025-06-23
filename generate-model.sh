#!/bin/bash
# Usage:
# ./generate_model.sh sqlite Client "id:INTEGER:PRIMARY KEY" "name:TEXT" "email:TEXT:UNIQUE" ...

DB_TYPE="$1"
TABLE_NAME="$2"
shift 2

if [[ -z "$DB_TYPE" || -z "$TABLE_NAME" || $# -eq 0 ]]; then
    echo "Usage: $0 <db_type> <TableName> <field:type[:constraint]> ..."
        echo 'axample : ./generate_model.sh sqlite Client "id:INTEGER:PRIMARY KEY" "name:TEXT" "email:TEXT:UNIQUE" "user_id:INTEGER:REFERENCES users(id) ON DELETE CASCADE" "rate:REAL" "created_at:DATETIME"'
    exit 1
fi

if [[ "$DB_TYPE" != "sqlite" ]]; then
    echo "Only sqlite is supported for now"
    exit 1
fi

HEADER_FILE="${TABLE_NAME,,}.hpp"
CLASS_NAME="$(tr '[:lower:]' '[:upper:]' <<< ${TABLE_NAME:0:1})${TABLE_NAME:1}"

FIELDS_DECLARATION=""
REGISTER_FIELDS=""
COPY_CONSTRUCTOR_PARTS=()
FIELD_COPY_INIT_PARTS=()
PARAM_CONSTRUCTOR_PARAMS=""
PARAM_CONSTRUCTOR_ASSIGN=""

for ARG in "$@"; do
    IFS=":" read -r FIELD TYPE CONSTRAINT <<< "$ARG"
    
    CPP_TYPE="std::string"
    [[ "$TYPE" == "INTEGER" ]] && CPP_TYPE="int"
    [[ "$TYPE" == "REAL" ]] && CPP_TYPE="float"
    [[ "$TYPE" == "DATETIME" ]] && CPP_TYPE="MhdDateTime"
    
    FIELDS_DECLARATION+="    $CPP_TYPE $FIELD;\n"
    REGISTER_FIELDS+="        fields.push_back({\"$FIELD\", \"$TYPE\", &$FIELD, \"${CONSTRAINT}\"});\n"
    COPY_CONSTRUCTOR_PARTS+=("        $FIELD(other.$FIELD)")
    FIELD_COPY_INIT_PARTS+=("        $FIELD($FIELD)")
    PARAM_CONSTRUCTOR_PARAMS+="const $CPP_TYPE& $FIELD, "
    PARAM_CONSTRUCTOR_ASSIGN+="        this->$FIELD = $FIELD;\n"
done

# Join arrays with commas and newlines
COPY_CONSTRUCTOR=""
for i in "${!COPY_CONSTRUCTOR_PARTS[@]}"; do
    if [[ $i -eq 0 ]]; then
        COPY_CONSTRUCTOR="${COPY_CONSTRUCTOR_PARTS[i]}"
    else
        COPY_CONSTRUCTOR="$COPY_CONSTRUCTOR,\n${COPY_CONSTRUCTOR_PARTS[i]}"
    fi
done

FIELD_COPY_INIT=""
for i in "${!FIELD_COPY_INIT_PARTS[@]}"; do
    if [[ $i -eq 0 ]]; then
        FIELD_COPY_INIT="${FIELD_COPY_INIT_PARTS[i]}"
    else
        FIELD_COPY_INIT="$FIELD_COPY_INIT,\n${FIELD_COPY_INIT_PARTS[i]}"
    fi
done

PARAM_CONSTRUCTOR_PARAMS="${PARAM_CONSTRUCTOR_PARAMS%, }"

# Generate the C++ header file
cat <<EOF > "Models/$HEADER_FILE"
#ifndef __${TABLE_NAME^^}_MODEL_H__
#define __${TABLE_NAME^^}_MODEL_H__

#include "../../utils/ORM/SQLiteORM/sqliteorm.hpp"
#include "../utils/datetime/mhd-date-time.hpp"  

class $CLASS_NAME : public SQLiteORM {
public:
$(echo -e "$FIELDS_DECLARATION")

    void registerFields() {
        fields.clear();
$(echo -e "$REGISTER_FIELDS")
    }

    $CLASS_NAME()
        : SQLiteORM("${TABLE_NAME,,}") {
        registerFields();
    }

    $CLASS_NAME(const $CLASS_NAME& other)
        : SQLiteORM(other)$( [[ -n "$COPY_CONSTRUCTOR" ]] && echo -e ",\n$COPY_CONSTRUCTOR" )
    {
        registerFields();
    }

    $CLASS_NAME($PARAM_CONSTRUCTOR_PARAMS)
        : SQLiteORM("${TABLE_NAME,,}")$( [[ -n "$FIELD_COPY_INIT" ]] && echo -e ",\n$FIELD_COPY_INIT" )
    {
        registerFields();
    }

    virtual ~$CLASS_NAME() = default;

    ORMModel* clone() const override {
        return new $CLASS_NAME(*this);
    }
};

#endif
EOF

echo -e "\e[32m✅ Model generated in : Models/$HEADER_FILE\e[0m"
echo -e "\e[33m⚠️  Don't forget to add it in the migrations.hpp file !!!\e[0m"