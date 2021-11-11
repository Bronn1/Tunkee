#pragma once

struct Objecttest {
	unsigned int identifier;
	bool operator==(const ObjectIdentifier&) const = default;
};