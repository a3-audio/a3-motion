"""How host.py finds the panel, without a panel. Run from firmware/:

    python3 -m unittest test_host
"""

import json
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace

import host


def port(device, vid=None, pid=None):
    return SimpleNamespace(device=device, vid=vid, pid=pid)


PANEL = (0x1A86, 0x55D3)


class PanelUsbIds(unittest.TestCase):
    def test_read_from_the_board_file(self):
        with tempfile.TemporaryDirectory() as tmp:
            board = Path(tmp) / "board.json"
            board.write_text(json.dumps(
                {"build": {"hwids": [["0x1A86", "0x55D3"]]}}))
            self.assertEqual({PANEL}, host.panel_usb_ids(board))

    def test_the_shipped_board_names_the_panel_bridge(self):
        self.assertIn(PANEL, host.panel_usb_ids())


class PanelCandidates(unittest.TestCase):
    def test_only_ports_with_the_panel_usb_id(self):
        ports = [port("/dev/ttyACM0", 0x2E8A, 0x000A),
                 port("/dev/ttyACM1", *PANEL),
                 port("/dev/ttyS0")]
        self.assertEqual(["/dev/ttyACM1"],
                         host.panel_candidates(ports, {PANEL}))

    def test_the_number_does_not_matter(self):
        ports = [port("/dev/ttyACM0", *PANEL)]
        self.assertEqual(["/dev/ttyACM0"],
                         host.panel_candidates(ports, {PANEL}))


class PickPanelPort(unittest.TestCase):
    def never(self, device):
        raise AssertionError("one candidate needs no ping: " + device)

    def test_one_candidate_is_taken_without_opening_it(self):
        self.assertEqual("/dev/ttyACM1",
                         host.pick_panel_port(["/dev/ttyACM1"], self.never))

    def test_of_several_the_one_answering_ping(self):
        answers = {"/dev/ttyACM0": False, "/dev/ttyACM1": True}
        self.assertEqual("/dev/ttyACM1", host.pick_panel_port(
            ["/dev/ttyACM0", "/dev/ttyACM1"], answers.get))

    def test_several_and_none_answering_is_an_error(self):
        with self.assertRaises(RuntimeError):
            host.pick_panel_port(["/dev/ttyACM0", "/dev/ttyACM1"],
                                 lambda device: False)

    def test_no_candidate_is_an_error_naming_the_usb_id(self):
        with self.assertRaises(RuntimeError) as caught:
            host.pick_panel_port([], self.never, {PANEL})
        self.assertIn("1A86:55D3", str(caught.exception))


if __name__ == "__main__":
    unittest.main()
