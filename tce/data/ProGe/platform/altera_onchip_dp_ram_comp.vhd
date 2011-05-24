-- megafunction wizard: %RAM: 2-PORT%
-- GENERATION: STANDARD
-- VERSION: WM1.0
-- MODULE: altsyncram 

-- ============================================================
-- File Name: koskenlaskija_altera_onchip_dp_ram_comp.vhd
-- Megafunction Name(s):
--          altsyncram
--
-- Simulation Library Files(s):
--          
-- ============================================================
-- ************************************************************
-- THIS IS A WIZARD-GENERATED FILE. DO NOT EDIT THIS FILE!
--
-- 8.0 Build 215 05/29/2008 SJ Full Version
-- ************************************************************


--Copyright (C) 1991-2008 Altera Corporation
--Your use of Altera Corporation's design tools, logic functions 
--and other software and tools, and its AMPP partner logic 
--functions, and any output files from any of the foregoing 
--(including device programming or simulation files), and any 
--associated documentation or information are expressly subject 
--to the terms and conditions of the Altera Program License 
--Subscription Agreement, Altera MegaCore Function License 
--Agreement, or other applicable license agreement, including, 
--without limitation, that your use is for the sole purpose of 
--programming logic devices manufactured by Altera and sold by 
--Altera or its authorized distributors.  Please refer to the 
--applicable agreement for further details.


library ieee;
use ieee.std_logic_1164.all;

library altera_mf;
use altera_mf.all;

entity ENTITY_STR_altera_onchip_dp_ram_comp is
  generic (
    init_file_g  : string  := "init_data.mif";
    dev_family_g : string  := "Cyclone II";
    addrw_g      : integer := 10;
    dataw_g      : integer := 32
    );
  port (
      address_a : in  std_logic_vector (addrw_g-1 downto 0);
      address_b : in  std_logic_vector (addrw_g-1 downto 0);
      byteena_a : in  std_logic_vector (dataw_g/8-1 downto 0);
      byteena_b : in  std_logic_vector (dataw_g/8-1 downto 0);
      clock_a   : in  std_logic;
      clock_b   : in  std_logic;
      data_a    : in  std_logic_vector (dataw_g-1 downto 0);
      data_b    : in  std_logic_vector (dataw_g-1 downto 0);
      enable_a  : in  std_logic;
      enable_b  : in  std_logic;
      wren_a    : in  std_logic;
      wren_b    : in  std_logic;
      q_a       : out std_logic_vector (dataw_g-1 downto 0);
      q_b       : out std_logic_vector (dataw_g-1 downto 0)
      );
end ENTITY_STR_altera_onchip_dp_ram_comp;


architecture SYN of ENTITY_STR_altera_onchip_dp_ram_comp is

  signal sub_wire0 : std_logic_vector (dataw_g-1 downto 0);
  signal sub_wire1 : std_logic_vector (dataw_g-1 downto 0);

  constant byte_size_c : integer := 8;
  constant bew_c       : integer := dataw_g/byte_size_c;

  component altsyncram
    generic (
      address_reg_b             : string;
      byteena_reg_b             : string;
      byte_size                 : natural;
      clock_enable_input_a      : string;
      clock_enable_input_b      : string;
      clock_enable_output_a     : string;
      clock_enable_output_b     : string;
      indata_reg_b              : string;
      init_file                 : string;
      intended_device_family    : string;
      lpm_type                  : string;
      numwords_a                : natural;
      numwords_b                : natural;
      operation_mode            : string;
      outdata_aclr_a            : string;
      outdata_aclr_b            : string;
      outdata_reg_a             : string;
      outdata_reg_b             : string;
      power_up_uninitialized    : string;
      widthad_a                 : natural;
      widthad_b                 : natural;
      width_a                   : natural;
      width_b                   : natural;
      width_byteena_a           : natural;
      width_byteena_b           : natural;
      wrcontrol_wraddress_reg_b : string
      );
    port (
      clocken0  : in  std_logic;
      clocken1  : in  std_logic;
      wren_a    : in  std_logic;
      clock0    : in  std_logic;
      wren_b    : in  std_logic;
      clock1    : in  std_logic;
      byteena_a : in  std_logic_vector (bew_c-1 downto 0);
      byteena_b : in  std_logic_vector (bew_c-1 downto 0);
      address_a : in  std_logic_vector (addrw_g-1 downto 0);
      address_b : in  std_logic_vector (addrw_g-1 downto 0);
      q_a       : out std_logic_vector (dataw_g-1 downto 0);
      q_b       : out std_logic_vector (dataw_g-1 downto 0);
      data_a    : in  std_logic_vector (dataw_g-1 downto 0);
      data_b    : in  std_logic_vector (dataw_g-1 downto 0)
      );
  end component;

begin
  q_a <= sub_wire0(dataw_g-1 downto 0);
  q_b <= sub_wire1(dataw_g-1 downto 0);

  altsyncram_component : altsyncram
    generic map (
      address_reg_b             => "CLOCK1",
      byteena_reg_b             => "CLOCK1",
      byte_size                 => byte_size_c,
      clock_enable_input_a      => "NORMAL",
      clock_enable_input_b      => "NORMAL",
      clock_enable_output_a     => "BYPASS",
      clock_enable_output_b     => "BYPASS",
      indata_reg_b              => "CLOCK1",
      init_file                 => init_file_g,
      intended_device_family    => dev_family_g,
      lpm_type                  => "altsyncram",
      numwords_a                => 2**addrw_g,
      numwords_b                => 2**addrw_g,
      operation_mode            => "BIDIR_DUAL_PORT",
      outdata_aclr_a            => "NONE",
      outdata_aclr_b            => "NONE",
      outdata_reg_a             => "UNREGISTERED",
      outdata_reg_b             => "UNREGISTERED",
      power_up_uninitialized    => "FALSE",
      widthad_a                 => addrw_g,
      widthad_b                 => addrw_g,
      width_a                   => dataw_g,
      width_b                   => dataw_g,
      width_byteena_a           => bew_c,
      width_byteena_b           => bew_c,
      wrcontrol_wraddress_reg_b => "CLOCK1"
      )
    port map (
      clocken0  => enable_a,
      clocken1  => enable_b,
      wren_a    => wren_a,
      clock0    => clock_a,
      wren_b    => wren_b,
      clock1    => clock_b,
      byteena_a => byteena_a,
      byteena_b => byteena_b,
      address_a => address_a,
      address_b => address_b,
      data_a    => data_a,
      data_b    => data_b,
      q_a       => sub_wire0,
      q_b       => sub_wire1
      );

end SYN;
