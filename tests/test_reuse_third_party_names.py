# SPDX-FileCopyrightText: 2026 Raphael Eismann
# SPDX-License-Identifier: GPL-3.0-or-later
"""Every foreign stanza of the REUSE data carries the name the ABOUT tree shows.

a3-core's ABOUT page (app/ui/SystemCheckout.cpp, thirdPartyOf) reads its
third-party node from the .reuse/dep5 of every repository in the tree: a stanza with a copyright holder who is not ours is
an entry, named by its X-A3-Name. Without one the page falls back to a
comment's first sentence, which reads badly; this holds every such stanza to
having its name. Stanzas of trivial files (CC0-1.0 only) are no entry.
"""

import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DEP5 = ROOT / ".reuse/dep5"

#: The copyright holder decided 2026-10-30, and Patric Schmitz, a project
#: member co-holding the mixer firmware; the same list as a3-core's SystemCheckout.cpp.
OUR_HOLDERS = {"raphael eismann", "raphael eismann and contributors", "patric schmitz",
               "a3 audio", "a3 audio contributors", "a3-audio contributors"}


def paragraphs(text):
    """The dep5's paragraphs: field -> its lines (first value, then continuations)."""
    result, current, field = [], {}, None
    for raw in text.splitlines():
        if not raw.strip():
            if current:
                result.append(current)
            current, field = {}, None
        elif raw[0] in " \t" and field:
            current[field].append(raw.strip())
        elif ":" in raw:
            field, value = raw.split(":", 1)
            field = field.strip()
            current[field] = [value.strip()]
    if current:
        result.append(current)
    return result


def holders(lines):
    names = []
    for line in lines:
        if "<" in line:
            line = line[:line.index("<")] + line[line.index(">") + 1:]
        line = line.strip().lstrip("0123456789-, ")
        if line.startswith("present"):
            line = line[len("present"):].lstrip(" ,")
        names += [part.strip() for part in line.split(",") if part.strip()]
    return names


def is_foreign(stanza):
    return any(h.lower() not in OUR_HOLDERS for h in holders(stanza.get("Copyright", [])))


def is_trivial(stanza):
    ids = stanza["License"][0].replace("(", " ").replace(")", " ").split()
    return all(word in ("CC0-1.0", "AND", "OR") for word in ids)


class ForeignStanzasAreNamed(unittest.TestCase):
    def test_every_foreign_stanza_has_a_name(self):
        unnamed = [" ".join(s["Files"]) for s in paragraphs(DEP5.read_text())
                   if "Files" in s and is_foreign(s) and not is_trivial(s) and not s.get("X-A3-Name", [""])[0]]
        self.assertEqual(unnamed, [], "foreign stanzas without X-A3-Name")

    def test_the_rule_sees_a_foreign_holder_and_not_ours(self):
        self.assertTrue(is_foreign({"Copyright": ["2018 Chris Johnson", "2023-2026 Raphael Eismann"]}))
        self.assertTrue(is_foreign({"Copyright": ["2014-present PlatformIO <contact@platformio.org>"]}))
        self.assertFalse(is_foreign({"Copyright": ["2024 Patric Schmitz, Raphael Eismann"]}))
        self.assertFalse(is_foreign({"Copyright": ["2023-2026 Raphael Eismann and contributors"]}))


if __name__ == "__main__":
    unittest.main()
