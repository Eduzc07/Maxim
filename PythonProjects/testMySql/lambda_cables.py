'''
=============================================
 Program to send data to MySql in lambda AWS
=============================================
'''
import json
import rds_config
import pymysql
import sys
from collections import OrderedDict

#rds settings
rds_host  = "e2idb.c0dc6ib7q6dp.us-west-2.rds.amazonaws.com"
name = rds_config.db_username
password = rds_config.db_password
db_name = rds_config.db_name
db_table = "iot_gateway"

# try:
#     conn = pymysql.connect(rds_host, user=name, passwd=password, db=db_name, connect_timeout=5)
# except pymysql.MySQLError as e:
#     #logger.error("ERROR: Unexpected error: Could not connect to MySQL instance.")
#     #logger.error(e)
#     print(e)
#     print('Exit!')
#     sys.exit()

def lambda_handler(event = None, context = None):
    """
    This function fetches content from MySQL RDS instance
    """

    #ID
    datetime = event["datetime"]
    machine_identifier = event["machine_identifier"]
    type_of_machine = event["type_of_machine"]

    #-------------------------------------------------------------
    # Grupo 1
    #-------------------------------------------------------------
    dipper_load = event["dipper_load"]
    current_truck_load = event["current_truck_load"]
    last_truck_load = event["last_truck_load"]
    shift_tons = event["shift_tons"]
    line_voltage =  event["line_voltage"]
    motion_control_kw = event["motion_control_kw"]
    power_factor = event["power_factor"]
    machine_kw = event["machine_kw"]
    transformer_1_temp = event["transformer_1_temp"]
    transformer_2_temp = event["transformer_2_temp"]

    #-------------------------------------------------------------
    # Grupo 2
    #-------------------------------------------------------------
    hoist_speed = event["hoist_speed"]
    hoist_torque = event["hoist_torque"]
    hoist_iw = event["hoist_iw"]
    hoist_kw = event["hoist_kw"]
    hoist_speed_ref = event["hoist_speed_ref"]
    hoist_motor_temp = event["hoist_motor_temp"]
    crowd_extension = event["crowd_extension"]
    hoist_rope_length = event["hoist_rope_length"]
    swing_angle = event["swing_angle"]
    igbt_module_1_temp = event["igbt_module_1_temp"]
    igbt_module_2_temp = event["igbt_module_2_temp"]
    igbt_module_3_temp = event["igbt_module_3_temp"]
    igbt_module_4_temp = event["igbt_module_4_temp"]
    igbt_module_5_temp = event["igbt_module_5_temp"]
    igbt_module_6_temp = event["igbt_module_6_temp"]
    igbt_module_7_temp = event["igbt_module_7_temp"]

    #-------------------------------------------------------------
    # Grupo 3
    #-------------------------------------------------------------
    hoist_drum_r_bear_temp = event["hoist_drum_r_bear_temp"]
    hoist_drum_l_bear_temp = event["hoist_drum_l_bear_temp"]
    hoist_up_in_pinion_bear_temp = event["hoist_up_in_pinion_bear_temp"]
    hoist_low_in_pinion_beat_temp = event["hoist_low_in_pinion_beat_temp"]
    hoist_mtr_r_bear_temp = event["hoist_mtr_r_bear_temp"]
    hoist_mtr_l_bear_temp = event["hoist_mtr_l_bear_temp"]
    hoist_gearcase_oil_temp = event["hoist_gearcase_oil_temp"]
    hoist_gearcase_oil_pressure = event["hoist_gearcase_oil_pressure"]
    crowd_speed = event["crowd_speed"]
    crowd_torque = event["crowd_torque"]
    crowd_iw = event["crowd_iw"]
    crowd_kw = event["crowd_kw"]
    crowd_speed_ref = event["crowd_speed_ref"]
    crowd_motor_temp = event["crowd_motor_temp"]
    igbt_module_8_temp = event["igbt_module_8_temp"]
    igbt_module_9_temp = event["igbt_module_9_temp"]
    igbt_module_10_temp = event["igbt_module_10_temp"]
    igbt_module_11_temp = event["igbt_module_11_temp"]
    igbt_module_12_temp = event["igbt_module_12_temp"]
    igbt_module_13_temp = event["igbt_module_13_temp"]
    igbt_module_14_temp = event["igbt_module_14_temp"]

    #-------------------------------------------------------------
    # Grupo 4
    #-------------------------------------------------------------
    beta_angle = event["beta_angle"]
    crowd_drum_r_bear_temp = event["crowd_drum_r_bear_temp"]
    crowd_drum_l_bear_temp = event["crowd_drum_l_bear_temp"]
    crowd_in_shaft_in_bear_temp = event["crowd_in_shaft_in_bear_temp"]
    crowd_in_shaft_out_bear_temp = event["crowd_in_shaft_out_bear_temp"]
    crowd_1_inter_shaft_in_bear_temp = event["crowd_1_inter_shaft_in_bear_temp"]
    crowd_1_inter_shaft_out_bear_temp = event["crowd_1_inter_shaft_out_bear_temp"]
    crowd_2_inter_shaft_in_bear_temp = event["crowd_2_inter_shaft_in_bear_temp"]
    crowd_2_inter_shaft_out_bear_temp = event["crowd_2_inter_shaft_out_bear_temp"]
    crowd_motor_r_bear_temp = event["crowd_motor_r_bear_temp"]
    crowd_motor_l_bear_temp = event["crowd_motor_l_bear_temp"]
    swing_speed = event["swing_speed"]
    swing_torque = event["swing_torque"]
    swing_iw = event["swing_iw"]
    swing_kw = event["swing_kw"]
    swing_ms_ref = event["swing_ms_ref"]
    swing_1_motor_temp = event["swing_1_motor_temp"]
    swing_2_motor_temp = event["swing_2_motor_temp"]

    #-------------------------------------------------------------
    # Grupo 5
    #-------------------------------------------------------------
    total_tilt = event["total_tilt"]
    fore_aft_tilt = event["fore_aft_tilt"]
    side_side_tilt = event["side_side_tilt"]
    cycle_swing_angle = event["cycle_swing_angle"]
    outside_ambient_air_temperature = event["outside_ambient_air_temperature"]
    house_ambient_air_temperature = event["house_ambient_air_temperature"]
    swing_gearcase_1_up_fore_bearing_temp = event["swing_gearcase_1_up_fore_bearing_temp"]
    swing_gearcase_1_low_aft_bearing_temp = event["swing_gearcase_1_low_aft_bearing_temp"]
    swing_mtr_1_up_bear_temp = event["swing_mtr_1_up_bear_temp"]
    swing_mtr_1_low_bear_temp = event["swing_mtr_1_low_bear_temp"]
    air_pressure = event["air_pressure"]
    lube_a1_pressure = event["lube_a1_pressure"]
    lube_a2_pressure = event["lube_a2_pressure"]
    lube_b1_pressure = event["lube_b1_pressure"]
    lube_b2_pressure = event["lube_b2_pressure"]
    lube_c1_pressure = event["lube_c1_pressure"]
    lube_c2_pressure = event["lube_c2_pressure"]

    #-------------------------------------------------------------
    # Grupo 6
    #-------------------------------------------------------------
    swing_gearcase_2_up_fore_bearing_temp = event["swing_gearcase_2_up_fore_bearing_temp"]
    swing_gearcase_2_low_aft_bearing_temp = event["swing_gearcase_2_low_aft_bearing_temp"]
    swing_mtr_2_up_bear_temp = event["swing_mtr_2_up_bear_temp"]
    swing_mtr_2_low_bear_temp = event["swing_mtr_2_low_bear_temp"]
    status_byte_1 = event["status_byte_1"]
    status_byte_2 = event["status_byte_2"]
    status_byte_3 = event["status_byte_3"]
    status_byte_4 = event["status_byte_4"]
    status_byte_5 = event["status_byte_5"]
    status_byte_6 = event["status_byte_6"]
    hc_states_byte_1 = event["hc_states_byte_1"]
    hc_states_byte_2 = event["hc_states_byte_2"]
    hc_states_byte_3 = event["hc_states_byte_3"]
    hc_states_byte_4 = event["hc_states_byte_4"]
    machine_operating_hours = event["machine_operating_hours"]
    operator_id = event["operator_id"]
    cycle_count = event["cycle_count"]
    hoist_fault_count = event["hoist_fault_count"]
    crowd_fault_count = event["crowd_fault_count"]
    swing_fault_count = event["swing_fault_count"]
    propel_fault_count = event["propel_fault_count"]
    boom_jack_fault_count = event["boom_jack_fault_count"]
    afe_fault_count = event["afe_fault_count"]
    idle_time = event["idle_time"]
    propel_time = event["propel_time"]
    dig_time = event["dig_time"]
    cycle_time = event["cycle_time"]
    new_truck_count = event["new_truck_count"]
    new_truck_time = event["new_truck_time"]

    #-------------------------------------------------------------
    # Grupo 7
    #-------------------------------------------------------------
    lube_a1_pressure_count = event["lube_a1_pressure_count"]
    lube_a2_pressure_count = event["lube_a2_pressure_count"]
    lube_a3_pressure_count = event["lube_a3_pressure_count"]
    lube_a4_pressure_count = event["lube_a4_pressure_count"]
    lube_a5_pressure_count = event["lube_a5_pressure_count"]
    lube_a6_pressure_count = event["lube_a6_pressure_count"]
    lube_a1_cycle_time = event["lube_a1_cycle_time"]
    lube_a2_cycle_time = event["lube_a2_cycle_time"]
    lube_a3_cycle_time = event["lube_a3_cycle_time"]
    lube_a4_cycle_time = event["lube_a4_cycle_time"]
    lube_a5_cycle_time = event["lube_a5_cycle_time"]
    lube_a6_cycle_time = event["lube_a6_cycle_time"]
    crowd_stop_limit_extension = event["crowd_stop_limit_extension"]
    ret_stop_limit_extension = event["ret_stop_limit_extension"]
    hoist_stop_limit_extension = event["hoist_stop_limit_extension"]
    lower_stop_limit_extension = event["lower_stop_limit_extension"]
    boom_jack_fault_extension = event["boom_jack_fault_extension"]
    spare_1 = event["spare_1"]
    spare_2 = event["spare_2"]
    spare_3 = event["spare_3"]
    spare_4 = event["spare_4"]
    spare_5 = event["spare_5"]
    spare_6 = event["spare_6"]
    spare_7 = event["spare_7"]
    spare_8 = event["spare_8"]

    #-------------------------------------------------------------
    # Grupo 8
    #-------------------------------------------------------------
    hc_estimated_warmup_time_remaining = event["hc_estimated_warmup_time_remaining"]
    cooler_inlet_temperature = event["cooler_inlet_temperature"]
    cooler_outlet_temperature = event["cooler_outlet_temperature"]
    lube_pump_gearcase_temperature = event["lube_pump_gearcase_temperature"]
    swing_1_bearing_5_temp = event["swing_1_bearing_5_temp"]
    swing_1_bearing_6_temp = event["swing_1_bearing_6_temp"]
    swing_1_bearing_7_temp = event["swing_1_bearing_7_temp"]
    swing_1_bearing_8_temp = event["swing_1_bearing_8_temp"]
    cylinder_manifold_outlet_temperature = event["cylinder_manifold_outlet_temperature"]
    lower_hydraulic_oil_temperature = event["lower_hydraulic_oil_temperature"]
    upper_hydraulic_oil_temperature = event["upper_hydraulic_oil_temperature"]
    swing_2_bearing_5_temp = event["swing_2_bearing_5_temp"]
    swing_2_bearing_6_temp = event["swing_2_bearing_6_temp"]
    swing_2_bearing_7_temp = event["swing_2_bearing_7_temp"]
    swing_2_bearing_8_temp = event["swing_2_bearing_8_temp"]
    warmup_sequence_1 = event["warmup_sequence_1"]
    warmup_sequence_2 = event["warmup_sequence_2"]

    #-------------------------------------------------------------
    # Grupo 9
    #-------------------------------------------------------------
    lube_pump_gearcase_pressure = event["lube_pump_gearcase_pressure"]
    return_filter_sensor_pressure = event["return_filter_sensor_pressure"]
    crowd_pressure_transducer_cap = event["crowd_pressure_transducer_cap"]
    crowd_pressure_transducer_rod = event["crowd_pressure_transducer_rod"]
    cylinder_manifold_outlet_pressure = event["cylinder_manifold_outlet_pressure"]
    pump_manifold_outlet_pressure = event["pump_manifold_outlet_pressure"]
    crowd_position_sensor_velocity = event["crowd_position_sensor_velocity"]
    crowd_position_sensor_position = event["crowd_position_sensor_position"]
    crowd_motor_tacometer_speed = event["crowd_motor_tacometer_speed"]
    crowd_master_switch_reference = event["crowd_master_switch_reference"]
    crowd_motor_torque = event["crowd_motor_torque"]
    plc_percent_ref_to_drive = event["plc_percent_ref_to_drive"]
    pump_case_drain_monitor_1 = event["pump_case_drain_monitor_1"]
    pump_case_drain_monitor_2 = event["pump_case_drain_monitor_2"]
    pump_case_drain_monitor_3 = event["pump_case_drain_monitor_3"]
    pump_case_drain_monitor_4 = event["pump_case_drain_monitor_4"]

    print("INSERT INTO %s,%s VALUES (NULL, %s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,----%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)",(db_name, db_table, datetime, machine_identifier, type_of_machine, dipper_load, current_truck_load, last_truck_load, shift_tons, line_voltage, motion_control_kw, power_factor, machine_kw, transformer_1_temp, transformer_2_temp, hoist_speed, hoist_torque, hoist_iw, hoist_kw, hoist_speed_ref, hoist_motor_temp, crowd_extension, hoist_rope_length, swing_angle, igbt_module_1_temp, igbt_module_2_temp, igbt_module_3_temp, igbt_module_4_temp, igbt_module_5_temp, igbt_module_6_temp, igbt_module_7_temp, hoist_drum_r_bear_temp, hoist_drum_l_bear_temp, hoist_up_in_pinion_bear_temp, hoist_low_in_pinion_beat_temp, hoist_mtr_r_bear_temp, hoist_mtr_l_bear_temp, hoist_gearcase_oil_temp, hoist_gearcase_oil_pressure, crowd_speed, crowd_torque, crowd_iw, crowd_kw, crowd_speed_ref, crowd_motor_temp, igbt_module_8_temp, igbt_module_9_temp, igbt_module_10_temp, igbt_module_11_temp, igbt_module_12_temp, igbt_module_13_temp, igbt_module_14_temp, beta_angle, crowd_drum_r_bear_temp, crowd_drum_l_bear_temp, crowd_in_shaft_in_bear_temp, crowd_in_shaft_out_bear_temp, crowd_1_inter_shaft_in_bear_temp, crowd_1_inter_shaft_out_bear_temp, crowd_2_inter_shaft_in_bear_temp, crowd_2_inter_shaft_out_bear_temp, crowd_motor_r_bear_temp, crowd_motor_l_bear_temp, swing_speed, swing_torque, swing_iw, swing_kw, swing_ms_ref, swing_1_motor_temp, swing_2_motor_temp, total_tilt, fore_aft_tilt, side_side_tilt, cycle_swing_angle, outside_ambient_air_temperature, house_ambient_air_temperature, swing_gearcase_1_up_fore_bearing_temp, swing_gearcase_1_low_aft_bearing_temp, swing_mtr_1_up_bear_temp, swing_mtr_1_low_bear_temp, air_pressure, lube_a1_pressure, lube_a2_pressure, lube_b1_pressure, lube_b2_pressure, lube_c1_pressure, lube_c2_pressure, swing_gearcase_2_up_fore_bearing_temp, swing_gearcase_2_low_aft_bearing_temp, swing_mtr_2_up_bear_temp, swing_mtr_2_low_bear_temp, status_byte_1, status_byte_2, status_byte_3, status_byte_4, status_byte_5, status_byte_6, hc_states_byte_1, hc_states_byte_2, hc_states_byte_3, hc_states_byte_4, machine_operating_hours, operator_id, cycle_count, hoist_fault_count, crowd_fault_count, swing_fault_count, propel_fault_count, boom_jack_fault_count, afe_fault_count, idle_time, propel_time, dig_time, cycle_time, new_truck_count, new_truck_time, lube_a1_pressure_count, lube_a2_pressure_count, lube_a3_pressure_count, lube_a4_pressure_count, lube_a5_pressure_count, lube_a6_pressure_count, lube_a1_cycle_time, lube_a2_cycle_time, lube_a3_cycle_time, lube_a4_cycle_time, lube_a5_cycle_time, lube_a6_cycle_time, crowd_stop_limit_extension, ret_stop_limit_extension, hoist_stop_limit_extension, lower_stop_limit_extension, boom_jack_fault_extension, spare_1, spare_2, spare_3, spare_4, spare_5, spare_6, spare_7, spare_8, hc_estimated_warmup_time_remaining, cooler_inlet_temperature, cooler_outlet_temperature, lube_pump_gearcase_temperature, swing_1_bearing_5_temp, swing_1_bearing_6_temp, swing_1_bearing_7_temp, swing_1_bearing_8_temp, cylinder_manifold_outlet_temperature, lower_hydraulic_oil_temperature, upper_hydraulic_oil_temperature, swing_2_bearing_5_temp, swing_2_bearing_6_temp, swing_2_bearing_7_temp, swing_2_bearing_8_temp, warmup_sequence_1, warmup_sequence_2, lube_pump_gearcase_pressure, return_filter_sensor_pressure, crowd_pressure_transducer_cap, crowd_pressure_transducer_rod, cylinder_manifold_outlet_pressure, pump_manifold_outlet_pressure, crowd_position_sensor_velocity, crowd_position_sensor_position, crowd_motor_tacometer_speed, crowd_master_switch_reference, crowd_motor_torque, plc_percent_ref_to_drive, pump_case_drain_monitor_1, pump_case_drain_monitor_2, pump_case_drain_monitor_3, pump_case_drain_monitor_4))
    print("aaa %d"%(2))

    Insert to MySql
    with conn.cursor() as cur:
       cur.execute("INSERT INTO db_rul.iot_gateway VALUES (NULL, %s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)",(datetime, machine_identifier, type_of_machine, dipper_load, current_truck_load, last_truck_load, shift_tons, line_voltage, motion_control_kw, power_factor, machine_kw, transformer_1_temp, transformer_2_temp, hoist_speed, hoist_torque, hoist_iw, hoist_kw, hoist_speed_ref, hoist_motor_temp, crowd_extension, hoist_rope_length, swing_angle, igbt_module_1_temp, igbt_module_2_temp, igbt_module_3_temp, igbt_module_4_temp, igbt_module_5_temp, igbt_module_6_temp, igbt_module_7_temp, hoist_drum_r_bear_temp, hoist_drum_l_bear_temp, hoist_up_in_pinion_bear_temp, hoist_low_in_pinion_beat_temp, hoist_mtr_r_bear_temp, hoist_mtr_l_bear_temp, hoist_gearcase_oil_temp, hoist_gearcase_oil_pressure, crowd_speed, crowd_torque, crowd_iw, crowd_kw, crowd_speed_ref, crowd_motor_temp, igbt_module_8_temp, igbt_module_9_temp, igbt_module_10_temp, igbt_module_11_temp, igbt_module_12_temp, igbt_module_13_temp, igbt_module_14_temp, beta_angle, crowd_drum_r_bear_temp, crowd_drum_l_bear_temp, crowd_in_shaft_in_bear_temp, crowd_in_shaft_out_bear_temp, crowd_1_inter_shaft_in_bear_temp, crowd_1_inter_shaft_out_bear_temp, crowd_2_inter_shaft_in_bear_temp, crowd_2_inter_shaft_out_bear_temp, crowd_motor_r_bear_temp, crowd_motor_l_bear_temp, swing_speed, swing_torque, swing_iw, swing_kw, swing_ms_ref, swing_1_motor_temp, swing_2_motor_temp, total_tilt, fore_aft_tilt, side_side_tilt, cycle_swing_angle, outside_ambient_air_temperature, house_ambient_air_temperature, swing_gearcase_1_up_fore_bearing_temp, swing_gearcase_1_low_aft_bearing_temp, swing_mtr_1_up_bear_temp, swing_mtr_1_low_bear_temp, air_pressure, lube_a1_pressure, lube_a2_pressure, lube_b1_pressure, lube_b2_pressure, lube_c1_pressure, lube_c2_pressure, swing_gearcase_2_up_fore_bearing_temp, swing_gearcase_2_low_aft_bearing_temp, swing_mtr_2_up_bear_temp, swing_mtr_2_low_bear_temp, status_byte_1, status_byte_2, status_byte_3, status_byte_4, status_byte_5, status_byte_6, hc_states_byte_1, hc_states_byte_2, hc_states_byte_3, hc_states_byte_4, machine_operating_hours, operator_id, cycle_count, hoist_fault_count, crowd_fault_count, swing_fault_count, propel_fault_count, boom_jack_fault_count, afe_fault_count, idle_time, propel_time, dig_time, cycle_time, new_truck_count, new_truck_time, lube_a1_pressure_count, lube_a2_pressure_count, lube_a3_pressure_count, lube_a4_pressure_count, lube_a5_pressure_count, lube_a6_pressure_count, lube_a1_cycle_time, lube_a2_cycle_time, lube_a3_cycle_time, lube_a4_cycle_time, lube_a5_cycle_time, lube_a6_cycle_time, crowd_stop_limit_extension, ret_stop_limit_extension, hoist_stop_limit_extension, lower_stop_limit_extension, boom_jack_fault_extension, spare_1, spare_2, spare_3, spare_4, spare_5, spare_6, spare_7, spare_8, hc_estimated_warmup_time_remaining, cooler_inlet_temperature, cooler_outlet_temperature, lube_pump_gearcase_temperature, swing_1_bearing_5_temp, swing_1_bearing_6_temp, swing_1_bearing_7_temp, swing_1_bearing_8_temp, cylinder_manifold_outlet_temperature, lower_hydraulic_oil_temperature, upper_hydraulic_oil_temperature, swing_2_bearing_5_temp, swing_2_bearing_6_temp, swing_2_bearing_7_temp, swing_2_bearing_8_temp, warmup_sequence_1, warmup_sequence_2, lube_pump_gearcase_pressure, return_filter_sensor_pressure, crowd_pressure_transducer_cap, crowd_pressure_transducer_rod, cylinder_manifold_outlet_pressure, pump_manifold_outlet_pressure, crowd_position_sensor_velocity, crowd_position_sensor_position, crowd_motor_tacometer_speed, crowd_master_switch_reference, crowd_motor_torque, plc_percent_ref_to_drive, pump_case_drain_monitor_1, pump_case_drain_monitor_2, pump_case_drain_monitor_3, pump_case_drain_monitor_4))

    conn.commit()
    out = "'Shovel' from RDS MySQL table has been added"

    return out

# Only for test
if __name__ == "__main__":
    print(__doc__)
    print('Starting!...')
    dataMQTT2 = {
        "id":"767edea0-0ea9-11eb-92ec-ef4cbf6fb91e",
        "datetime":"2020-10-15 05:44:16",
        "machine_identifier":"400",
        "type_of_machine":"2055",
        "dipper_load":0,
        "current_truck_load":0,
        "last_truck_load":395,
        "shift_tons":278,
        "line_voltage":7.199999809265137,
        "motion_control_kw":-1842,
        "power_factor":-100,
        "machine_kw":-1674,
        "transformer_1_temp":152,
        "transformer_2_temp":156,
        "hoist_speed":1263,
        "hoist_torque":-72,
        "hoist_iw":-1171,
        "hoist_kw":-1852,
        "hoist_speed_ref":-18432,
        "hoist_motor_temp":94,
        "crowd_extension":23.314373016357422,
        "hoist_rope_length":45.35674285888672,
        "swing_angle":11,
        "igbt_module_1_temp":25,
        "igbt_module_2_temp":21,
        "igbt_module_3_temp":23,
        "igbt_module_4_temp":32,
        "igbt_module_5_temp":30,
        "igbt_module_6_temp":30,
        "igbt_module_7_temp":25,
        "hoist_drum_r_bear_temp":23,
        "hoist_drum_l_bear_temp":37,
        "hoist_up_in_pinion_bear_temp":63,
        "hoist_low_in_pinion_beat_temp":18,
        "hoist_mtr_r_bear_temp":25,
        "hoist_mtr_l_bear_temp":36,
        "hoist_gearcase_oil_temp":67,
        "hoist_gearcase_oil_pressure":119,
        "crowd_speed":-1312
    }

    dataMQTT = {
        #ID
        "id": "afawgwer34g3g34g34g",
        "datetime": "2020-10-05 22:59:05",
        "machine_identifier": "Shovel01",
        "type_of_machine": "495HR",

        #-------------------------------------------------------------
        # Grupo 1
        #-------------------------------------------------------------
        "dipper_load": 0,
        "current_truck_load": 105,
        "last_truck_load": 486,
        "shift_tons": 6877,
        "line_voltage": 6.9120001792907715,
        "motion_control_kw": 2714,
        "power_factor": 100,
        "machine_kw": 2664,
        "transformer_1_temp": 89,
        "transformer_2_temp": 35,

        #-------------------------------------------------------------
        # Grupo 2
        #-------------------------------------------------------------
        "hoist_speed": 0,
        "hoist_torque": -86,
        "hoist_iw": -1347,
        "hoist_kw": 2156,
        "hoist_speed_ref": 1536,
        "hoist_motor_temp": 89,
        "crowd_extension": 16823,
        "hoist_rope_length": 16774,
        "swing_angle": 126,
        "igbt_module_1_temp": 23,
        "igbt_module_2_temp": 21,
        "igbt_module_3_temp": 21,
        "igbt_module_4_temp": 32,
        "igbt_module_5_temp": 28,
        "igbt_module_6_temp": 28,
        "igbt_module_7_temp": 23,

        #-------------------------------------------------------------
        # Grupo 3
        #-------------------------------------------------------------
        "hoist_drum_r_bear_temp": 24,
        "hoist_drum_l_bear_temp": 41,
        "hoist_up_in_pinion_bear_temp": 37,
        "hoist_low_in_pinion_beat_temp": 64,
        "hoist_mtr_r_bear_temp": 26,
        "hoist_mtr_l_bear_temp": 36,
        "hoist_gearcase_oil_temp": 67,
        "hoist_gearcase_oil_pressure": 117,
        "crowd_speed": 2104,
        "crowd_torque": 105,
        "crowd_iw": 156,
        "crowd_kw": 15,
        "crowd_speed_ref": 27648,
        "crowd_motor_temp": 93,
        "igbt_module_8_temp": 21,
        "igbt_module_9_temp": 21,
        "igbt_module_10_temp": 30,
        "igbt_module_11_temp": 28,
        "igbt_module_12_temp": 28,
        "igbt_module_13_temp": 9,
        "igbt_module_14_temp": 7,

        #-------------------------------------------------------------
        # Grupo 4
        #-------------------------------------------------------------
        "beta_angle": 16172,
        "crowd_drum_r_bear_temp": 32,
        "crowd_drum_l_bear_temp": 19,
        "crowd_in_shaft_in_bear_temp": 82,
        "crowd_in_shaft_out_bear_temp": 64,
        "crowd_1_inter_shaft_in_bear_temp": 54,
        "crowd_1_inter_shaft_out_bear_temp": 57,
        "crowd_2_inter_shaft_in_bear_temp": 33,
        "crowd_2_inter_shaft_out_bear_temp": 42,
        "crowd_motor_r_bear_temp": 51,
        "crowd_motor_l_bear_temp": 44,
        "swing_speed": 1305,
        "swing_torque": 111,
        "swing_iw": 273,
        "swing_kw": 20,
        "swing_ms_ref": 22528,
        "swing_1_motor_temp": 85,
        "swing_2_motor_temp": 95,

        #-------------------------------------------------------------
        # Grupo 5
        #-------------------------------------------------------------
        "total_tilt": 16072,
        "fore_aft_tilt": 16072,
        "side_side_tilt": 15429,
        "cycle_swing_angle": 48,
        "outside_ambient_air_temperature": 16636,
        "house_ambient_air_temperature": 0,
        "swing_gearcase_1_up_fore_bearing_temp": 12,
        "swing_gearcase_1_low_aft_bearing_temp": 23,
        "swing_mtr_1_up_bear_temp": 27,
        "swing_mtr_1_low_bear_temp": 44,
        "air_pressure": 117,
        "lube_a1_pressure": 9,
        "lube_a2_pressure": 31,
        "lube_b1_pressure": 5,
        "lube_b2_pressure": 13,
        "lube_c1_pressure": 36,
        "lube_c2_pressure": 18,

        #-------------------------------------------------------------
        # Grupo 6
        #-------------------------------------------------------------
        "swing_gearcase_2_up_fore_bearing_temp": 22,
        "swing_gearcase_2_low_aft_bearing_temp": 22,
        "swing_mtr_2_up_bear_temp": 36,
        "swing_mtr_2_low_bear_temp": 54,
        "status_byte_1":189,
        "status_byte_2":59,
        "status_byte_3":254,
        "status_byte_4":0,
        "status_byte_5":184,
        "status_byte_6":5,
        "hc_states_byte_1": 255,
        "hc_states_byte_2": 10,
        "hc_states_byte_3": 10,
        "hc_states_byte_4": 10,
        "machine_operating_hours": 0,
        "operator_id": 0,
        "cycle_count": 0,
        "hoist_fault_count": 18235,
        "crowd_fault_count": 4608,
        "swing_fault_count": 9728,
        "propel_fault_count": 105,
        "boom_jack_fault_count": 106,
        "afe_fault_count": 107,
        "idle_time": 108,
        "propel_time": 109,
        "dig_time": 110,
        "cycle_time": 111,
        "new_truck_count": 112,
        "new_truck_time": 113,

        #-------------------------------------------------------------
        # Grupo 7
        #-------------------------------------------------------------
        "lube_a1_pressure_count": 114,
        "lube_a2_pressure_count": 115,
        "lube_a3_pressure_count": 116,
        "lube_a4_pressure_count": 117,
        "lube_a5_pressure_count": 118,
        "lube_a6_pressure_count": 119,
        "lube_a1_cycle_time": 120,
        "lube_a2_cycle_time": 121,
        "lube_a3_cycle_time": 122,
        "lube_a4_cycle_time": 123,
        "lube_a5_cycle_time": 124,
        "lube_a6_cycle_time": 125,
        "crowd_stop_limit_extension": 126,
        "ret_stop_limit_extension": 127,
        "hoist_stop_limit_extension": 128,
        "lower_stop_limit_extension": 129,
        "boom_jack_fault_extension": 130,
        "spare_1": 131.9,
        "spare_2": 132.1,
        "spare_3": 133.4,
        "spare_4": 134.5,
        "spare_5": 135.9,
        "spare_6": 136.2,
        "spare_7": 137.5,
        "spare_8": 138.1,

        #-------------------------------------------------------------
        # Grupo 8
        #-------------------------------------------------------------
        "hc_estimated_warmup_time_remaining": 139.9,
        "cooler_inlet_temperature": 140.2,
        "cooler_outlet_temperature": 141.2,
        "lube_pump_gearcase_temperature": 142.4,
        "swing_1_bearing_5_temp": 143.4,
        "swing_1_bearing_6_temp": 144.7,
        "swing_1_bearing_7_temp": 145.1,
        "swing_1_bearing_8_temp": 146.2,
        "cylinder_manifold_outlet_temperature": 147.9,
        "lower_hydraulic_oil_temperature": 148.1,
        "upper_hydraulic_oil_temperature": 149.3,
        "swing_2_bearing_5_temp": 150.2,
        "swing_2_bearing_6_temp": 151.1,
        "swing_2_bearing_7_temp": 152.2,
        "swing_2_bearing_8_temp": 153.1,
        "warmup_sequence_1": 154.9,
        "warmup_sequence_2": 155.2,

        #-------------------------------------------------------------
        # Grupo 9
        #-------------------------------------------------------------
        "lube_pump_gearcase_pressure": 156.9,
        "return_filter_sensor_pressure": 157.9,
        "crowd_pressure_transducer_cap": 158.9,
        "crowd_pressure_transducer_rod": 159.1,
        "cylinder_manifold_outlet_pressure": 160.9,
        "pump_manifold_outlet_pressure": 161.2,
        "crowd_position_sensor_velocity": 162.1,
        "crowd_position_sensor_position": 163.2,
        "crowd_motor_tacometer_speed": 164.2,
        "crowd_master_switch_reference": 165.2,
        "crowd_motor_torque": 166.2,
        "plc_percent_ref_to_drive": 167.2,
        "pump_case_drain_monitor_1": 168.2,
        "pump_case_drain_monitor_2": 169.2,
        "pump_case_drain_monitor_3": 170.2,
        "pump_case_drain_monitor_4": 171.1
    }

    #print(dataMQTT)

    # printing initial json
    ini_string = json.dumps(dataMQTT)
    #print ("initial 1st dictionary", ini_string)
    #print ("type of ini_object", type(ini_string))

    # converting string to json
    event = json.loads(ini_string, object_pairs_hook=OrderedDict)

    # printing final result
    #print ("final dictionary", str(event))
    #print ("type of final_dictionary", type(event))

    #print(changeDate("28/05/2020 16:22:53"))
    print(lambda_handler(event = event))
