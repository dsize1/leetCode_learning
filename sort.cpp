#include <random>
#include <iostream>
#include <ctime>

using namespace std;

unsigned random_gen(int min, int max);
unsigned random_gen(int min, int max, unsigned seed);
vector<unsigned> random_gen(int min, int max, unsigned seed, int num);

void fast_sort (vector<unsigned> &vec);
void fast_sort (vector<unsigned> &vec, int l, int r);

void merge_sort (vector<unsigned> &vec);

void heap_sort (vector<unsigned> &vec);

template <typename T>
ostream& log_vec (ostream& os, vector<T> &vec, const string &title);

int main () {
	int vec_size = 20;
	vector<unsigned> random_li = random_gen(0, 99, time(0), vec_size);
	log_vec(cout, random_li, "vector");
	fast_sort(random_li);
	log_vec(cout, random_li, "quick sort result");

	random_li = random_gen(0, 99, time(0), vec_size);
	merge_sort(random_li);
	log_vec(cout, random_li, "merge sort result");

	random_li = random_gen(0, 99, time(0), vec_size);
	heap_sort(random_li);
	log_vec(cout, random_li, "heap sort result");
	return 0;
}

unsigned random_gen(int min, int max)
{
	std::default_random_engine e;
	std::uniform_int_distribution<unsigned> ud(min, max);
	return ud(e);
}

unsigned random_gen(int min, int max, unsigned seed)
{
	std::default_random_engine e_seed(seed);
	std::uniform_int_distribution<unsigned> ud_seed(min, max);
	return ud_seed(e_seed);
}

vector<unsigned> random_gen(int min, int max, unsigned seed, int num)
{
	std::default_random_engine e_vec(seed);
	std::uniform_int_distribution<unsigned> ud_vec(min, max);
	vector<unsigned> re(num, 0);
	for (auto beg = re.begin(), end = re.end();
		beg != end; ++beg) {
		*beg = ud_vec(e_vec);
	}
	return re;
}

template <typename T>
ostream& log_vec (ostream &os, vector<T> &vec, const string &title)
{
	os << "==========" << endl;
	os << title <<":" << endl;
	for (const auto & num : vec) {
		os << num << " ";
	}
	os << endl;
	os << "==========" << endl;
	return os;
}

int partition (vector<unsigned> &vec, int l, int r)
{
	int random_index = random_gen(l, r);
	unsigned pivot = vec[random_index];
	swap(vec[l], vec[random_index]);
	while(l < r) {
		while(l < r && pivot <= vec[r])
			--r;
		vec[l] = vec[r];
		while(l < r && vec[l] <= pivot)
			++l;
		vec[r] = vec[l];
	}
	vec[l] = pivot;
	return l;
}

void fast_sort (vector<unsigned> &vec, int l, int r)
{
	if (r > l) {
		int p = partition(vec, l, r);
		fast_sort(vec, l, p - 1);
		fast_sort(vec, p + 1, r);
	} 
}

void fast_sort (vector<unsigned> &vec)
{
	fast_sort(vec, 0, vec.size() - 1);
}

vector<unsigned> merge_fn (vector<unsigned> vec1, vector<unsigned> vec2)
{
	vector<unsigned> merged;
	int i = 0, j = 0, size1 = vec1.size(), size2 = vec2.size();
	while(i < size1 && j < size2) {
		if (vec1[i] < vec2[j]) {
			merged.push_back(vec1[i++]);
		} else {
			merged.push_back(vec2[j++]);
		}
	}
	while(i < size1) {
		merged.push_back(vec1[i++]);
	}
	while(j < size2) {
		merged.push_back(vec2[j++]);
	}
	return merged;
}

vector<unsigned> merge_sort_fn (vector<unsigned> vec)
{
	if (vec.size() == 1) {
		return vec;
	}
	int mid = (vec.size()) / 2;
	auto merged1 = merge_sort_fn(vector<unsigned>(vec.begin(), vec.begin() + mid));
	auto merged2 = merge_sort_fn(vector<unsigned>(vec.begin() + mid, vec.end()));
	return merge_fn(merged1, merged2);
}

void merge_sort (vector<unsigned> &vec)
{
	vec = merge_sort_fn(vec);	
}

class Min_priority_queue {
	public:
		Min_priority_queue(vector<unsigned> &v): vec({0}) {
			for(auto beg = v.begin(), end = v.end();
				beg != end; ++beg) {
				add(*beg);
			}
		}

		unsigned min() {
			return vec[1];
		}

		void add(unsigned num) {
			vec.push_back(num);
			swim(vec.size() - 1);
		}

		unsigned delete_min() {
			unsigned deleted = vec[1];
			swap(vec[1], vec[vec.size() - 1]);
			vec.pop_back();
			sink(1);
			return deleted;
		}

	private:
		void sink(int k) {
			int l = k * 2, r = k * 2 + 1;
			if (l >= vec.size()) {
				return;
			}
			if (less(l, k) || (r < vec.size() && less(r, k))) {
				if(r >= vec.size() || less(l, r)) {
					swap(vec[k], vec[l]);
					sink(l);
				} else {
					swap(vec[k], vec[r]);
					sink(r);
				}
			}
		}

		void swim(int k) {
			if (k > 1) {
				int p = k / 2;
				int b = k % 2 == 0 ? k + 1 : k - 1;
				if (less(k, p) && (b == vec.size() || less(k, b))) {
					swap(vec[k], vec[p]);
					swim(p);
				} else if (b < vec.size() && less(b, p) && less(b, k)) {
					swap(vec[b], vec[p]);
					swim(p);
				}
			}
		}

		int less(int i, int j) {
			return vec[i] < vec[j];
		}

		vector<unsigned> vec;
};

void heap_sort (vector<unsigned> &vec)
{
	Min_priority_queue minPQ(vec);
	for(auto beg = vec.begin(), end = vec.end();
		beg != end; ++beg) {
		*beg = minPQ.delete_min();
	}
}