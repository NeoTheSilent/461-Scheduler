#include <iostream>
#include <iomanip>
#include <string>
#include <math.h>
#include <random>
#include <array>

using namespace std;

std::array<string, 6> teacher_hare = { "CS 101", "CS 201", "CS 291", "CS 303", "CS 449", "CS 461" };
std::array<string, 6> teacher_bingham = { "CS 101", "CS 201", "CS 191", "CS 291", "CS 449" };
std::array<string, 6> teacher_kuhail = { "CS 303", "CS 341" };
std::array<string, 6> teacher_mitchell = { "CS 191", "CS 291", "CS 303", "CS 341" };
std::array<string, 6> teacher_rao = { "CS 291", "CS 303", "CS 341", "CS 461" };
std::array<string, 7> rooms = { "Haag 301", "Haag 206", "Royall 204", "Katz 209", "Flarsheim 310", "Flarsheim 260", "Bloch 0009" };
std::array <string, 12> expected_courses = { "CS 101", "CS 101", "CS 201", "CS 201", "CS 191", "CS 191", "CS 291", "CS 291", "CS 303", "CS 341", "CS 449", "CS 461" };
std::array <int, 12> expected_enrollment = { 40, 25, 30, 30, 60, 20, 40, 20, 50, 40, 55, 40 };
std::array <int, 7> time_slots = { 10, 11, 12, 13, 14, 15, 16 };



struct Course {
	std::array<string, 6> teacher;
	string course;
	string room;
	string building;
	int room_size;
	int enrolled;
	int time;

	Course() {
		teacher = { "", "", "", "", "", "" };
		course = "";
		room = "";
		building = "";
		room_size = -1;
		enrolled = -1;
		time = -1;
	}

	Course(std::array<string, 6> teacher2, string course2, string room2, int enroll, int time2) {
		teacher = teacher2;
		course = course2;
		room = room2;
		building = Building(room);
		room_size = RoomSize(room);
		enrolled = enroll;
		time = time2;
	}

	string Teacher() {
		if (teacher == teacher_bingham) { return "Bingham"; }
		else if (teacher == teacher_hare) { return "Hare"; }
		else if (teacher == teacher_kuhail) { return "Kuhail"; }
		else if (teacher == teacher_mitchell) { return "Mitchell"; }
		else if (teacher == teacher_rao) { return "Rao"; }
		else { return "Error"; }
	}

	string Building(string room) {
		if ((room == "Haag 301") || (room == "Haag 206")) { return "Haag"; }
		else if (room == "Royall 204") { return "Royall"; }
		else if (room == "Katz 209") { return "Katz"; }
		else if ((room == "Flarsheim 310") || (room == "Flarsheim 260")) { return "Flarsheim"; }
		else if (room == "Bloch 0009") { return "Bloch"; }
	}

	int RoomSize(string course) {
		if (course == "Haag 301") { return 70; }
		else if (course == "Haag 206") { return 30; }
		else if (course == "Royall 204") { return 70; }
		else if (course == "Katz 209") { return 50; }
		else if (course == "Flarsheim 310") { return 80; }
		else if (course == "Flarsheim 260") { return 25; }
		else if (course == "Bloch 0009") { return 30; }
	}
};

struct Schedule {
	std::array<Course, 12> course;
	
	int qualified_teacher(int fitness) {
		for (int i = 0; i < course.size(); i++) {
			for (int j = 0; j < course[i].teacher.size(); j++) {
				if (course[i].course == course[i].teacher[j]) {
					fitness += 3;
					j = 7;
				}
			}
		}
		return fitness;
	}

	int only_class(int fitness) {
		for (int i = 0; i < course.size(); i++) {
			bool only = true;
			for (int j = 0; j < course.size(); j++) {
				if (((course[i].room == course[j].room) && (i != j))
					&& (course[i].time == course[j].time)) {
					only = false;
				}
			}
			if (only == true) {
				fitness += 5;
			}
		}
		return fitness;
	}

	int class_size(int fitness) {
		for (int i = 0; i < course.size(); i++) {
			if (course[i].room_size >= course[i].enrolled) {
				fitness += 5;
				if (course[i].room_size <= (2 * course[i].enrolled)) {
					fitness += 2;
				}
			}
		}
		return fitness;
	}

	int simul_teaching(int fitness) {
		for (int i = 0; i < course.size(); i++) {
			bool conflict = false;
			for (int j = 0; j < course.size(); j++) {
				if ((i != j) && (course[i].teacher == course[j].teacher)) {
					if (course[i].time == course[j].time) {
						conflict = true;
						j = course.size();
					}
				}
			}
			if (conflict == false) {
				fitness += 5;
			}
		}
		return fitness;
	}

	int overwork(int fitness) {
		int h = 0;
		int b = 0;
		int k = 0;
		int m = 0;
		int r = 0;
		for (int i = 0; i < course.size(); i++) {
			if (course[i].teacher == teacher_hare) { h++; }
			else if (course[i].teacher == teacher_bingham) { b++; }
			else if (course[i].teacher == teacher_kuhail) { k++; }
			else if (course[i].teacher == teacher_mitchell) { m++; }
			else if (course[i].teacher == teacher_rao) { r++; }
		}
		if (h > 4) { fitness -= ((h - 4) * 5); }
		if (b > 4) { fitness -= ((b - 4) * 5); }
		if (k > 4) { fitness -= ((k - 4) * 5); }
		if (m > 4) { fitness -= ((m - 4) * 5); }
		if (r > 4) { fitness -= ((r - 4) * 5); }

		int total_problems = 1;
		if (r > b) { total_problems -= 0.05; }
		if (r > h) { total_problems -= 0.05; }
		if (m > b) { total_problems -= 0.05; }
		if (m > h) { total_problems -= 0.05; }
		fitness *= total_problems;
		return fitness;
	}

	int compliment(int fitness) {
		int total_problems = 1;
		for (int i = 0; i < course.size(); i++) {
			for (int j = 0; j < course.size(); j++) {
				if (((course[i].course == "CS 101") && (course[j].course == "CS 191")) ||
					((course[i].course == "CS 201") && (course[j].course == "CS 291"))) {

					if (course[i].time == course[j].time) {
						total_problems -= 0.10;
					}
					else if (((course[i].time + 1) == course[j].time) || ((course[i].time - 1) == course[j].time)) {
						total_problems += 0.05;
						if (course[i].room == course[j].room) {
							fitness += 5;
						}
						if ((course[i].room == "Katz") && (course[j].room != "Katz")) {
							total_problems -= 0.03;
						}
						if ((course[i].room == "Bloch") && (course[j].room != "Bloch")) {
							total_problems -= 0.03;
						}
					}
				}
			}
		}
		return fitness *= total_problems;
	}

	// Extra Credit 1
	int early_mornings(int fitness) {
		for (int i = 0; i < course.size(); i++) {
			// Since Hare loves early mornings, let's make reward early classes
			if (((course[i].Teacher() == "Hare") || (course[i].Teacher() == "Bingham")) && (course[i].time < 12)) {
				fitness += 3;
			}
		}
		return fitness;
	}

	// Extra Credit 2
	int right_area(int fitness) {
		for (int i = 0; i < course.size(); i++) {
			//Certain professors love the quad, so let's make sure that they're on them with rewards
			if (((course[i].Teacher() == "Hare" || (course[i].Teacher() == "Bingham")) || (course[i].Teacher() == "Kuhail"))
				&& ((course[i].building == "Haag") || (course[i].building == "Royall") || (course[i].building == "Flarsheim"))) {
				fitness += 2;
			}
		}
		return fitness;
	}

	int cal_fitness()
	{
		int fitness = 0;
		// If a teacher is qualified to teach that class
		fitness = qualified_teacher(fitness);


		// If a class is the only one being taught in that room at that time
		fitness = only_class(fitness);

		// If a classroom is big enough for a class, but no more than 2x
		fitness = class_size(fitness);

		// Is the same teacher teaching multiple classes simultaneously?
		fitness = simul_teaching(fitness);

		//Extra Credit
		fitness = early_mornings(fitness);
		fitness = right_area(fitness);

		// Is the same teacher teaching too much?
		// Furthermore, Is Rao or Mitchell teaching more Hare or Bingham
		// ASSUMPTION: We count every instance of this being a problem, and make it additive instead of multiplicative
		fitness = overwork(fitness);

		//Are CS101 and 191, or CS201 and 291 being scheduled optimally?
		//ASSUMPTION: We count every good or bad instance additively, and add any flat numbers before adding the multiplier
		fitness = compliment(fitness);
		
		//If a fitness is weak, we're just setting it to 0 here to save us time
		if (fitness < 0) { fitness = 0; }

		return fitness;
	}

	void constraint_violator() {

		cout << endl;
		for (int i = 0; i < course.size(); i++) {
			bool qual = false;
			for (int j = 0; j < teacher_bingham.size(); j++) {
				if (course[i].course == course[i].teacher[j]) {
					qual = true;
				}
			}
			if (qual == false) {
				cout << course[i].Teacher() << " not qualified to teach " << course[i].course << endl;
			}
		}

		cout << endl;
		for (int i = 0; i < course.size(); i++) {
			for (int j = 0; j < course.size(); j++) {
				if (((course[i].room == course[j].room) && ((i != j) && (i < j)))
					&& (course[i].time == course[j].time)) {
					cout << "Class " << i << " and " << j << " are scheduled at the same spot and same time" << endl;
				}
			}
		}
		cout << endl;

		for (int i = 0; i < course.size(); i++) {
			if (course[i].room_size < course[i].enrolled) {
				cout << "Class " << i << " has a room that's too small" << endl;
			}
			else if (course[i].room_size > (2 * course[i].enrolled)) {
				cout << "Class " << i << " has a room that's too big" << endl;
			}
		}
		cout << endl;

		for (int i = 0; i < course.size(); i++) {
			for (int j = 0; j < course.size(); j++) {
				if (((i != j) && (i < j)) && (course[i].teacher == course[j].teacher)) {
					if (course[i].time == course[j].time) {
						cout << course[i].Teacher() << " is teaching multiple classes at the same time" << endl;
						j = course.size();
					}
				}
			}
		}

		int h = 0; 
		int b = 0;
		int k = 0; 
		int m = 0; 
		int r = 0;
		for (int i = 0; i < course.size(); i++) {
			if (course[i].teacher == teacher_hare) { h++; }
			else if (course[i].teacher == teacher_bingham) { b++; }
			else if (course[i].teacher == teacher_kuhail) { k++; }
			else if (course[i].teacher == teacher_mitchell) { m++; }
			else if (course[i].teacher == teacher_rao) { r++; }
		}
		if (h > 4) { cout << "Hare is overworked and is teaching " << h << " classes" << endl; }
		if (b > 4) { cout << "Bingham is overworked and is teaching " << b << " classes" << endl; }
		if (k > 4) { cout << "Kuhail is overworked and is teaching " << k << " classes" << endl; }
		if (m > 4) { cout << "Mitchell is overworked and is teaching " << m << " classes" << endl; }
		if (r > 4) { cout << "Rao is overworked and is teaching " << r << " classes" << endl; }

		int total_problems = 1;
		if (r > b) { cout << "Rao is teaching more classes than Bingham" << endl; }
		if (r > h) { cout << "Rao is teaching more classes than Hare" << endl; }
		if (m > b) { cout << "Mitchell is teaching more classes than Bingham" << endl; }
		if (m > h) { cout << "Mitchell is teaching more classes than Hare" << endl; }
	}
};

struct NextGeneration {
	std::array<Schedule, 200> Generation;
	random_device machie;
	int generation_count;

	NextGeneration() {
		//Generation 0, Fully Randomized
		for (int i = 0; i < Generation.size(); i++) {
			for (int j = 0; j < Generation[i].course.size(); j++) {
				int n = (machie() % 12);
				Generation[i].course[j].course = expected_courses[n];
				Generation[i].course[j].enrolled = expected_enrollment[n];
				Generation[i].course[j].room = rooms[machie() % 7];
				Generation[i].course[j].building = Generation[i].course[j].Building(Generation[i].course[j].room);
				Generation[i].course[j].room_size = Generation[i].course[j].RoomSize(Generation[i].course[j].room);
				Generation[i].course[j].teacher = teacher_randomizer();
				Generation[i].course[j].time = time_slots[machie() % 7];
			}
			generation_count = 0;
		}
	}

	int best_fit() {
		int best = 0;
		for (int i = 0; i < Generation.size(); i++)
			if (best < Generation[i].cal_fitness()) {
				best = Generation[i].cal_fitness();
			}
		return best;
	}

	array <string, 6> teacher_randomizer() {
		int m = (machie() % 5);
		if (m == 0) { return teacher_bingham; }
		else if (m == 1) { return teacher_hare; }
		else if (m == 2) { return teacher_kuhail; }
		else if (m == 3) { return teacher_mitchell; }
		else if (m == 4) { return teacher_rao; }
	}

	Schedule create_child(Schedule spouse1, Schedule spouse2) {
		Schedule child;
		random_device machie;
		for (int i = 0; i < spouse1.course.size(); i++) {
			int ran = machie() % 100;
			if (ran <= 44) {
				for (int i = 0; i < spouse1.course.size(); i++) {
					child.course[i] = spouse1.course[i];
				}
			}
			else if (ran <= 89) {
				for (int i = 0; i < spouse2.course.size(); i++) {
					child.course[i] = spouse2.course[i];
				}
			}
			else {
				ran = machie() % 4;
				if (ran == 0) {
					int course_info = machie() % 12;
					child.course[i].course = expected_courses[course_info];
					child.course[i].enrolled = expected_enrollment[course_info];
				}
				else if (ran == 1) {
					child.course[i].room = rooms[machie() % 7];
					child.course[i].building = child.course[i].Building(child.course[i].room);
					child.course[i].room_size = child.course[i].RoomSize(child.course[i].room);
				}
				else if (ran == 2) { child.course[i].teacher = teacher_randomizer(); }
				else if (ran == 3) { child.course[i].time = time_slots[machie() % 7]; }
			}
		}
		return child;
	}

	void Next() {

		// We're staying here until we reach a conclusion, so might as well take a seat and get comfy.
		// Numbers are initialized like this so we don't instantly end.
		int fitness = 100;
		int father_fitness = 1;
		int grand_fitness = 100;
		int best = best_fit();
		bool loop = true;

		/*while (loop = true) {

			if (((((fitness / father_fitness) <= 1.02) || ((fitness / father_fitness) >= 0.98)) &&
				(((fitness / grand_fitness) <= 1.02) || ((fitness / grand_fitness) >= 0.98))) &&
				(((grand_fitness / father_fitness) <= 1.02) || ((grand_fitness / father_fitness) >= 0.98))) {*/
		for (int i = 0; i < 10; i++) {
			// I sure wish we could save the best guaranteed instead of leaving it up to chance
			// Anyways, L2 randomization time. 
			double storage[200];
			double sum = 0;
			Schedule spouse1;
			Schedule spouse2;

			cout << "Before: " << fitness << " " << father_fitness << " " << grand_fitness << endl;
			fitness = father_fitness;
			father_fitness = grand_fitness;
			if (best_fit() != 0) {
				grand_fitness = best_fit();
			}
			else {
				grand_fitness = 1;
			}
			cout << "After: " << fitness << " " << father_fitness << " " << grand_fitness << endl;

			for (int i = 0; i < Generation.size(); i++) {
				storage[i] = Generation[i].cal_fitness();
				sum += storage[i];
				cout << storage[i] << " ";
			}
			cout << endl;
			// Since we need to report average and best fitness of each generation, let's go and do that here since 
			// We have them saved.

			cout << "Generation: " << generation_count << endl << "Current Average: " << (sum / Generation.size()) << endl;
			cout << "Best of Generation: ";
			best = best_fit();
			cout << best << endl << endl;


			for (int i = 0; i < Generation.size(); i++) {
				storage[i] = pow(storage[i], 2);
				storage[i] /= sum;
				storage[i] *= 1000;
				storage[i] = round(storage[i]);

				// This is also to help select a random number. The math behind it is this:
				// Let's say I have numbers 1, 2, 3, 4, 5. In turn, I have 1, 4, 9, 16 and 25 for a total of 55.
				// For example, 25/55 = .4545, but if I were to manually use it like I wished, it would mess up the 
				// results and artificially inflate it's score, while also creating a sixth spot.
				// However, if I add to them every time I'm past the first area (as I can't add what doesn't exist)
				// then I get the result where everything adds up to 1000, and I can simply select the first instance where
				// the array's number is bigger than the random number, as that will be what should be chosen.
				// I.e: In this example with 55 and 25, picking 900 would pick arr[5] since it's below the number associated with it
				// (1000). Another example: For 1, the magical number is 0.018 (or 18), so if the random number was 17, then the
				// number is 1. If it was 19, then the number was 2.
				// NOTE: I did not see the cumulative distribution. Please assume I have a brain
				if (i != 0) {
					storage[i] += storage[i - 1];
				}
			}
			// Now to randomly pick. For reiteration, I've already said how I'm picking it in the big paragraph 4 lines up
			// Now we pick our first of two numbers to save
			int ran = machie() % 1000;
			for (int i = 0; i < Generation.size(); i++) {
				if (storage[i] >= ran) {
					spouse1 = Generation[i];
					ran = machie() % 1000;
					// Now to pick our second number.
					for (int j = 0; j < Generation.size(); j++) {
						//We can't have the same number being picked, so we need to make sure that the first target isn't it
						if ((storage[j] >= ran) && (i == j)) {
							//If the first target is the same as the second, we reroll and restart
							j = 0;
							ran = machie() % 1000;
						}
						else if ((storage[j] >= ran) && (i != j)) {
							//We move on and end the loop while savig the right spouse
							spouse2 = Generation[j];
							j = Generation.size();
						}
						else if ((j == Generation.size()) && (storage[j] < ran)) {
							j = 0;
							ran = machie() % 1000;
						}
					}
					//With this in place, it'll instantly end the loop so only the first valid target gets hit
					i = Generation.size();
				}
				else if ((i == Generation.size()) && (storage[i] < ran)) {
					i = 0;
					ran = machie() % 1000;
				}
			}

			//Now let's move everything to the next generation.
			Generation[0] = spouse1;
			Generation[1] = spouse2;
			generation_count++;
			//We now have the spouses create children that are part each of them, with some randomization
			for (int i = 2; i < Generation.size(); i++) {
				cout << "Creating Child " << i << " of Generation " << generation_count << endl;
				Generation[i] = create_child(spouse1, spouse2);
			}
		}
		//and now, we've got a new generation, so let's restart, assuming that we haven't made a mistake;
			/*}
			else {
				loop = false;
			}
		}*/
	
		system("CLS");
		cout << "Program has stopped as there was no substantial growth for 3 generations, last generation is Gen " << generation_count << endl;
		cout << "Best Fitness Score is " << best_fit() << endl;
		cout << "This is the final schedule: " << endl;

		int best_schedule = 0;
		fitness = 0;
		for (int i = 0; i < Generation.size(); i++){
			if (fitness < Generation[i].cal_fitness()) {
				fitness = Generation[i].cal_fitness();
				best_schedule = i;
			}
		}

		for (int i = 0; i < Generation[best_schedule].course.size(); i++) {
			cout << "Class " << i << ":\t";
			cout << "Prof. " << Generation[best_schedule].course[i].Teacher() << " ";
			cout << "in " << Generation[best_schedule].course[i].room;
			cout << "\t\tRoom Size (" << Generation[best_schedule].course[i].room_size << ") ";
			cout << "Course: " << Generation[best_schedule].course[i].course << " ";
			cout << "Enrolled (" << Generation[best_schedule].course[i].enrolled << ") ";
			cout << "Time: " << Generation[best_schedule].course[i].time << ":00" << endl;
		}

		//Error Time
		Generation[best_schedule].constraint_violator();
		system("pause");
	}
};

int main() {
	
	// Declaring this here randomly generates the entire first generation.
	NextGeneration main;
	main.Next();
	return 0;
}