#ifndef FREQUENCY_TABLE_H
#define FREQUENCY_TABLE_H

#include <map>
#include <vector>


namespace data {
	template<typename T>
	class FrequencyTable {
		std::map<T, unsigned> frequencies;

	public:
		void increment(const T& x) {
			frequencies[x]++;
		}

		const std::vector<T> values() const {
			std::vector<T> values;
			values.reserve(frequencies.size());

			for (const auto& pair : frequencies) {
				values.push_back(pair.first);
			}

			return values;
		}

		const unsigned operator [](const T& x) {
			return (frequencies.find(x) != frequencies.end()) ? frequencies[x] : 0;
		}
	};

	template<typename T>
	FrequencyTable<T> count_frequencies(const std::vector<T>& values) {
		FrequencyTable<T> frequencies;

		for (const T& x : values) {
			frequencies.increment(x);
		}

		return frequencies;
	}
}

#endif