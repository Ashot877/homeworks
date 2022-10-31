#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>

// calculating physical size
int physical_size(int fd){

	int ph_size = 0;

	// cursor -> first position
	lseek(fd, 0, SEEK_SET);

	// cursor -> last position
	ph_size = lseek(fd, 0, SEEK_END);

	// check errors
	if(ph_size < 0){
		exit(errno);
	}

	// cursor -> first position
	lseek(fd, 0, SEEK_SET);

	return ph_size;
}




// calculating logical size
int logical_size(int fd){
	// logicak size counter
	int size = 0;

	int start = 0;
	int end =0;

	// cursor -> first position
	int position = lseek(fd, 0, SEEK_SET);

	// calculating data bytes
	while(position != lseek(fd, 0, SEEK_END)){

		// cursor -> the nearest data
		position = lseek(fd, 0, SEEK_DATA);

		// check errors
		if(position < 0){
			exit(errno);
		}

		// save start of the data
		start = position;

		// cursor -> the nearest hole
		position = lseek(fd, 0, SEEK_HOLE);

		// check errors
		if(position < 0){
			exit(errno);
		}

		// save  end of the data
		end = position;

		// add  data size to logic_size
		size += (end - start);
	}

	// cursor -> the first position
	lseek(fd, 0, SEEK_SET);

	return size;
}


int main(int argc, char **argv){

	// checking arguments
	if(argc < 3){
		std::cerr << "arguments are not enough" << std::endl;
		return 1;
	}

	// saving source and destination paths
	const char* source = argv[1];
	const char* destination = argv[2];

	// opening source path
	int source_fd = open(source, O_RDONLY);

	// checking for issues
	if(source_fd < 0){
		exit(errno);
	}

	// opening destination path
	int destination_fd = open(destination, O_CREAT | O_TRUNC | O_WRONLY, 0644);

	// checking for issues
	if(destination_fd < 0){
		exit(errno);
	}

	std::cout << physical_size(source_fd) << std::endl;
	std::cout << logical_size(source_fd) << std::endl;

	// creating buffer
	char buffer[4096];

	while(true){

		int count = read(source_fd, buffer, 4096);

		if(count == 0){
			break;
		}

		if(count < 0){
			return 1;
		}

		int write_count = write(destination_fd, buffer, count);

		if(write_count < 0){
			exit(errno);
		}

	}

	std::cout << physical_size(destination_fd) << std::endl;
	std::cout << logical_size(destination_fd) << std::endl;

	close(source_fd);

	close(destination_fd);

	return 0;
}
