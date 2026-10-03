"""The PCB test in host.py (--pcb-test, 2026-10-03): every button lights its
own LED, every encoder click turns the colour round the wheel and every lit
LED takes the new colour. For checking a freshly built board: what moves is
printed, and what it lights shows whether its LED is wired.

    python3 -m unittest test_host_pcb      (from firmware/)
"""

import unittest

import host

UP, DOWN = 1, 0          # a button's debounced level, as the firmware reports it


def states(*down):
    return [DOWN if i in down else UP for i in range(44)]


def led_of(label):
    return host.LED_BUTTON_LABELS.index(label)


class TheColourWheel(unittest.TestCase):
    def test_it_starts_red(self):
        self.assertEqual(host.pcb_colour(0), (255, 0, 0))

    def test_a_third_round_is_green(self):
        self.assertEqual(host.pcb_colour(host.PCB_STEPS // 3), (0, 255, 0))

    def test_it_comes_round(self):
        self.assertEqual(host.pcb_colour(host.PCB_STEPS), host.pcb_colour(0))
        self.assertEqual(host.pcb_colour(-1), host.pcb_colour(host.PCB_STEPS - 1))


class ButtonsLightTheirLed(unittest.TestCase):
    def test_a_press_lights_that_buttons_led(self):
        test = host.PcbTest()
        button = host.BUTTON_LABELS.index("21")
        commands = test.buttons(states(button))
        self.assertEqual(commands, [(led_of("21"), (255, 0, 0))])

    def test_a_second_press_puts_it_out(self):
        test = host.PcbTest()
        button = host.BUTTON_LABELS.index("21")
        test.buttons(states(button))
        test.buttons(states())
        self.assertEqual(test.buttons(states(button)), [(led_of("21"), (0, 0, 0))])

    def test_holding_is_one_press(self):
        test = host.PcbTest()
        button = host.BUTTON_LABELS.index("21")
        test.buttons(states(button))
        self.assertEqual(test.buttons(states(button)), [])


class EncodersTurnTheColour(unittest.TestCase):
    def test_a_click_recolours_every_lit_led(self):
        test = host.PcbTest()
        test.buttons(states(host.BUTTON_LABELS.index("21"), host.BUTTON_LABELS.index("00")))
        commands = test.encoders([0, 0, 1, 0, 0, 0, 0, 0])
        colour = host.pcb_colour(1)
        self.assertEqual(sorted(commands), sorted([(led_of("21"), colour), (led_of("00"), colour)]))

    def test_any_encoder_turns_it_and_back(self):
        test = host.PcbTest()
        test.buttons(states(host.BUTTON_LABELS.index("21")))
        test.encoders([0, 0, 0, 0, 0, 0, 0, 3])
        self.assertEqual(test.encoders([-3, 0, 0, 0, 0, 0, 0, 0]),
                         [(led_of("21"), host.pcb_colour(0))])

    def test_no_turn_sends_nothing(self):
        test = host.PcbTest()
        test.buttons(states(host.BUTTON_LABELS.index("21")))
        self.assertEqual(test.encoders([0] * 8), [])


if __name__ == "__main__":
    unittest.main()
