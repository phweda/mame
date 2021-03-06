// license:BSD-3-Clause
// copyright-holders:AJR
/***********************************************************************************************************************************

    Skeleton driver for E-mu Emulator Three (EIII) synthesizer.

***********************************************************************************************************************************/

#include "emu.h"
#include "cpu/ns32000/ns32000.h"
#include "machine/pit8253.h"
#include "machine/wd_fdc.h"
#include "machine/z80scc.h"

class emu3_state : public driver_device
{
public:
	emu3_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_maincpu(*this, "maincpu")
	{
	}

	void emu3(machine_config &config);

private:
	void mem_map(address_map &map);

	required_device<cpu_device> m_maincpu;
};


void emu3_state::mem_map(address_map &map)
{
	map(0x000000, 0x007fff).rom().region("bootprom", 0);
	map(0x008000, 0x027fff).ram();
	map(0x050000, 0x050007).w("fdc", FUNC(wd1772_device::write)).umask16(0x00ff);
	map(0x050008, 0x05000f).r("fdc", FUNC(wd1772_device::read)).umask16(0x00ff);
	//map(0x300000, 0x30000f).rw("hdc", FUNC(ncr5380n_device::read), FUNC(ncr5830n_device::write)).umask16(0x00ff);
	map(0x390000, 0x390007).rw("timer", FUNC(pit8254_device::read), FUNC(pit8254_device::write)).umask16(0x00ff);
	map(0x400000, 0xbfffff).ram();
}


static INPUT_PORTS_START(emu3)
INPUT_PORTS_END

void emu3_state::emu3(machine_config &config)
{
	NS32016(config, m_maincpu, 20_MHz_XTAL / 2); // 32016-10 CPU + 32001-10 TCU
	m_maincpu->set_addrmap(AS_PROGRAM, &emu3_state::mem_map);

	//NS32081(config, "fpu", 20_MHz_XTAL / 2);

	//R6500_11(config, "scannercpu", 16_MHz_XTAL / 4);

	WD1772(config, "fdc", 16_MHz_XTAL / 2);

	pit8254_device &pit(PIT8254(config, "timer")); // 8254-2
	pit.set_clk<0>(20_MHz_XTAL / 2);
	pit.set_clk<1>(16_MHz_XTAL / 16);

	//NCR5380N(config, "hdc");

	SCC85230(config, "uart", 16_MHz_XTAL / 4);
}

ROM_START(emu3)
	ROM_REGION16_LE(0x8000, "bootprom", 0)
	ROM_LOAD16_BYTE("e3-lsboot_ip381a_emu_systems_4088.ic3", 0x0000, 0x4000, CRC(34e5283f) SHA1(902c2a9a2b37b34331fb57d45b88ffabc1f12a53)) // 27128B
	ROM_LOAD16_BYTE("e3-msboot_ip380a_emu_systems_4088.ic4", 0x0001, 0x4000, CRC(1302c054) SHA1(28b7e8991e72cc111ee0067c58bddafca70f3824)) // 27128B

	ROM_REGION(0xc00, "scannercpu", 0)
	ROM_LOAD("im368.ic31", 0x000, 0xc00, NO_DUMP)
ROM_END

SYST(1987, emu3, 0, 0, emu3, emu3, emu3_state, empty_init, "E-mu Systems", "Emulator Three Digital Sound Production System", MACHINE_IS_SKELETON)
