

int main() {
	while (true) {
		processInput();

		reload();
		this_time = clock();

		time_counter += (double)(this_time - last_time);
		last_time = this_time;

		if (time_counter > tick) {
			cout << time_counter << endl;
			time_counter = 0;
		}
		else {
			cout << tick - time_counter;
			SDL_Delay(tick-time_counter);
		}
			
		
	}
}