<!-- polybuild-definition-schema.xsd -->
<?xml version="1.0" encoding="UTF-8"?>
<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema"
           xmlns:pbd="http://schema.obinexus.org/polybuild/definition/1.0"
           targetNamespace="http://schema.obinexus.org/polybuild/definition/1.0"
           elementFormDefault="qualified">

  <!-- Root element: PolyBuild Definition -->
  <xs:element name="definition" type="pbd:DefinitionType"/>

  <!-- Definition Type -->
  <xs:complexType name="DefinitionType">
    <xs:sequence>
      <xs:element name="identity" type="pbd:IdentityType"/>
      <xs:element name="topology" type="pbd:TopologyType"/>
      <xs:element name="intents" type="pbd:IntentsType" minOccurs="0"/>
      <xs:element name="actions" type="pbd:ActionsType" minOccurs="0"/>
      <xs:element name="source" type="pbd:SourceType" minOccurs="0"/>
      <xs:element name="dependencies" type="pbd:DependenciesType" minOccurs="0"/>
    </xs:sequence>
    <xs:attribute name="name" type="xs:string" use="required"/>
    <xs:attribute name="version" type="xs:string" use="required"/>
  </xs:complexType>

  <!-- Identity Type -->
  <xs:complexType name="IdentityType">
    <xs:sequence>
      <xs:element name="name" type="xs:string"/>
      <xs:element name="version" type="xs:string"/>
      <xs:element name="stability" type="pbd:StabilityType"/>
      <xs:element name="guid" type="xs:string"/>
    </xs:sequence>
  </xs:complexType>

  <!-- Stability Type -->
  <xs:simpleType name="StabilityType">
    <xs:restriction base="xs:string">
      <xs:enumeration value="experimental"/>
      <xs:enumeration value="beta"/>
      <xs:enumeration value="stable"/>
      <xs:enumeration value="legacy"/>
    </xs:restriction>
  </xs:simpleType>

  <!-- Topology Type -->
  <xs:complexType name="TopologyType">
    <xs:sequence>
      <xs:element name="fault-tolerance" type="pbd:FaultToleranceType"/>
      <xs:element name="concurrency" type="pbd:ConcurrencyType"/>
      <xs:element name="binary-encoding" type="pbd:BinaryEncodingType"/>
    </xs:sequence>
    <xs:attribute name="type" type="pbd:TopologyTypeEnum" use="required"/>
  </xs:complexType>

  <!-- Topology Type Enumeration -->
  <xs:simpleType name="TopologyTypeEnum">
    <xs:restriction base="xs:string">
      <xs:enumeration value="p2p"/>
      <xs:enumeration value="bus"/>
      <xs:enumeration value="star"/>
      <xs:enumeration value="ring"/>
      <xs:enumeration value="mesh"/>
    </xs:restriction>
  </xs:simpleType>

  <!-- Fault Tolerance Type -->
  <xs:complexType name="FaultToleranceType">
    <xs:attribute name="level" use="required">
      <xs:simpleType>
        <xs:restriction base="xs:integer">
          <xs:minInclusive value="0"/>
          <xs:maxInclusive value="12"/>
        </xs:restriction>
      </xs:simpleType>
    </xs:attribute>
  </xs:complexType>

  <!-- Concurrency Type -->
  <xs:complexType name="ConcurrencyType">
    <xs:attribute name="model" type="pbd:ConcurrencyModel" use="required"/>
  </xs:complexType>

  <!-- Concurrency Model -->
  <xs:simpleType name="ConcurrencyModel">
    <xs:restriction base="xs:string">
      <xs:enumeration value="sequential"/>
      <xs:enumeration value="parallel"/>
      <xs:enumeration value="pipeline"/>
    </xs:restriction>
  </xs:simpleType>

  <!-- Binary Encoding Type (7-bit pattern) -->
  <xs:simpleType name="BinaryEncodingType">
    <xs:restriction base="xs:string">
      <xs:pattern value="[01]{7}"/>
    </xs:restriction>
  </xs:simpleType>

  <!-- Intents Type -->
  <xs:complexType name="IntentsType">
    <xs:sequence>
      <xs:element name="intent" type="pbd:IntentType" maxOccurs="unbounded"/>
    </xs:sequence>
  </xs:complexType>

  <!-- Intent Type -->
  <xs:complexType name="IntentType">
    <xs:sequence>
      <xs:element name="stage" type="pbd:IntentStageType" minOccurs="0"/>
      <xs:element name="priority" type="xs:integer" minOccurs="0"/>
    </xs:sequence>
    <xs:attribute name="expression" type="pbd:IntentExpressionType" use="required"/>
  </xs:complexType>

  <!-- Intent Expression Type (verb-noun-binding pattern) -->
  <xs:simpleType name="IntentExpressionType">
    <xs:restriction base="xs:string">
      <xs:pattern value="(validate|build|compile|link|test|deploy|clean|reroute|configure)\s+(policy|target|source|dependency|artifact|pipeline|configuration|manifest)(\s+\w+)?"/>
    </xs:restriction>
  </xs:simpleType>

  <!-- Intent Stage Type -->
  <xs:simpleType name="IntentStageType">
    <xs:restriction base="xs:string">
      <xs:enumeration value="todo"/>
      <xs:enumeration value="doing"/>
      <xs:enumeration value="done"/>
    </xs:restriction>
  </xs:simpleType>

  <!-- Actions Type -->
  <xs:complexType name="ActionsType">
    <xs:sequence>
      <xs:element name="action" type="pbd:ActionType" maxOccurs="unbounded"/>
    </xs:sequence>
  </xs:complexType>

  <!-- Action Type -->
  <xs:complexType name="ActionType">
    <xs:sequence>
      <xs:element name="command" type="xs:string"/>
      <xs:element name="inputs" type="pbd:InputsType" minOccurs="0"/>
      <xs:element name="outputs" type="pbd:OutputsType" minOccurs="0"/>
    </xs:sequence>
    <xs:attribute name="name" type="xs:string" use="required"/>
  </xs:complexType>

  <!-- Inputs/Outputs Types -->
  <xs:complexType name="InputsType">
    <xs:sequence>
      <xs:element name="input" maxOccurs="unbounded">
        <xs:complexType>
          <xs:attribute name="path" type="xs:string" use="required"/>
        </xs:complexType>
      </xs:element>
    </xs:sequence>
  </xs:complexType>

  <xs:complexType name="OutputsType">
    <xs:sequence>
      <xs:element name="output" maxOccurs="unbounded">
        <xs:complexType>
          <xs:attribute name="path" type="xs:string" use="required"/>
        </xs:complexType>
      </xs:element>
    </xs:sequence>
  </xs:complexType>

  <!-- Source Type -->
  <xs:complexType name="SourceType">
    <xs:sequence>
      <xs:element name="include" type="xs:string" maxOccurs="unbounded"/>
      <xs:element name="exclude" type="xs:string" minOccurs="0" maxOccurs="unbounded"/>
    </xs:sequence>
    <xs:attribute name="root" type="xs:string" use="required"/>
  </xs:complexType>

  <!-- Dependencies Type -->
  <xs:complexType name="DependenciesType">
    <xs:sequence>
      <xs:element name="dependency" type="pbd:DependencyType" maxOccurs="unbounded"/>
    </xs:sequence>
  </xs:complexType>

  <!-- Dependency Type -->
  <xs:complexType name="DependencyType">
    <xs:attribute name="name" type="xs:string" use="required"/>
    <xs:attribute name="version" type="xs:string" use="required"/>
  </xs:complexType>

</xs:schema>

<!-- Example: complete-polybuild-definition.xml -->
<?xml version="1.0" encoding="UTF-8"?>
<pbd:definition 
    xmlns:pbd="http://schema.obinexus.org/polybuild/definition/1.0"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://schema.obinexus.org/polybuild/definition/1.0 polybuild-definition-schema.xsd"
    name="intent-driven-component"
    version="1.0.0">
    
    <!-- Component Identity -->
    <pbd:identity>
        <pbd:name>intent-resolver-core</pbd:name>
        <pbd:version>1.0.0</pbd:version>
        <pbd:stability>stable</pbd:stability>
        <pbd:guid>550e8400-e29b-41d4-a716-446655440000</pbd:guid>
    </pbd:identity>
    
    <!-- Build Topology Definition (Binary: 0101101) -->
    <pbd:topology type="star">
        <pbd:fault-tolerance level="6" />
        <pbd:concurrency model="parallel" />
        <pbd:binary-encoding>0101101</pbd:binary-encoding>
    </pbd:topology>
    
    <!-- Intent-Driven Declarations -->
    <pbd:intents>
        <pbd:intent expression="validate policy live">
            <pbd:stage>todo</pbd:stage>
            <pbd:priority>100</pbd:priority>
        </pbd:intent>
        
        <pbd:intent expression="reroute pipeline beta">
            <pbd:stage>todo</pbd:stage>
            <pbd:priority>80</pbd:priority>
        </pbd:intent>
        
        <pbd:intent expression="build target release">
            <pbd:stage>todo</pbd:stage>
            <pbd:priority>90</pbd:priority>
        </pbd:intent>
        
        <pbd:intent expression="compile source optimized">
            <pbd:stage>todo</pbd:stage>
            <pbd:priority>85</pbd:priority>
        </pbd:intent>
        
        <pbd:intent expression="test artifact comprehensive">
            <pbd:stage>todo</pbd:stage>
            <pbd:priority>70</pbd:priority>
        </pbd:intent>
    </pbd:intents>
    
    <!-- Source Definition -->
    <pbd:source root="./src">
        <pbd:include>**/*.c</pbd:include>
        <pbd:include>**/*.h</pbd:include>
        <pbd:include>intent/**/*.xml</pbd:include>
        <pbd:exclude>**/test/**</pbd:exclude>
        <pbd:exclude>**/deprecated/**</pbd:exclude>
    </pbd:source>
    
    <!-- Dependency Relationships -->
    <pbd:dependencies>
        <pbd:dependency name="dag-core" version="^1.0.0" />
        <pbd:dependency name="xml-parser" version="~2.1.0" />
        <pbd:dependency name="semantic-validator" version="^1.2.0" />
    </pbd:dependencies>
    
    <!-- Build Actions (Mapped from Intents) -->
    <pbd:actions>
        <pbd:action name="validate-semantics">
            <pbd:command>$(VALIDATOR) $(XML_MANIFEST) --strict</pbd:command>
            <pbd:inputs>
                <pbd:input path="$(MANIFEST_FILES)" />
            </pbd:inputs>
            <pbd:outputs>
                <pbd:output path="$(BUILD_DIR)/validation.report" />
            </pbd:outputs>
        </pbd:action>
        
        <pbd:action name="resolve-intents">
            <pbd:command>$(INTENT_RESOLVER) $(INTENT_MANIFEST) --topology=0101101</pbd:command>
            <pbd:inputs>
                <pbd:input path="$(INTENT_FILES)" />
            </pbd:inputs>
            <pbd:outputs>
                <pbd:output path="$(BUILD_DIR)/resolved_intents.dag" />
            </pbd:outputs>
        </pbd:action>
        
        <pbd:action name="compile-with-topology">
            <pbd:command>$(COMPILER) $(SOURCE) -o $(OUTPUT) --topology-aware</pbd:command>
            <pbd:inputs>
                <pbd:input path="$(SOURCE_FILES)" />
            </pbd:inputs>
            <pbd:outputs>
                <pbd:output path="$(BUILD_DIR)/$(TARGET).o" />
            </pbd:outputs>
        </pbd:action>
    </pbd:actions>
</pbd:definition>

<!-- polybuild.in configuration file -->
# PolyBuild Intent-Driven Configuration
# Topology Binary Encoding: 0101101 (Star/Parallel/FT6)

@directive [plugin:intent-resolver]
input.manifest = polybuild-definition.xml
input.topology_encoding = 0101101
input.semantic_validation = strict

@directive [plugin:xml-validator]
input.schema = polybuild-definition-schema.xsd
input.namespace = http://schema.obinexus.org/polybuild/definition/1.0

# Intent Resolution Pipeline
action.parse_intents
action.validate_semantics  
action.resolve_dependencies
action.execute_todo_stage
action.transition_doing_stage
action.complete_done_stage

# Topology-Aware Build Actions
action.compile_with_star_topology
action.parallel_link_phase
action.fault_tolerant_test

# Semantic Layer Enforcement
semantic.strict_validation = true
semantic.namespace_aware = true
semantic.verb_noun_patterns = true
semantic.stage_transitions = true
