#include <algorithm>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#define NO_TESTS (9)
#define BUF_SIZE (4096)

static std::string
exec(const char* cmd)
{
    char buffer[BUF_SIZE];
    FILE* pipe = popen(cmd, "r");
    if (!pipe) exit(1);

    size_t z = fread(buffer, 1, BUF_SIZE - 1, pipe);
    pclose(pipe);

    buffer[z] = 0;
    return std::string{ buffer };
}

int
main(void)
{
    std::vector<int> sizes = { 800, 1000, 1200, };

    for (auto&& test_size : sizes)
    {
        std::vector<double> parsing_results;
        std::vector<double> processing_results;

        for (int i = 0; i < NO_TESTS; ++i)
        {
            std::string output = exec(("./latc ./big-test" + std::to_string(test_size) + std::string{".lat"}).c_str());
            // printf("Out: \"%s\"", output.c_str());

            char const* p = output.c_str() + std::string{"parsing: "}.size();
            double d = atof(p);
            parsing_results.push_back(d);

            p = strchr(p, '\n');
            p += std::string{"processing: "}.size();

            d = atof(p);
            processing_results.push_back(d);
        }

        std::sort(parsing_results.begin(), parsing_results.end());
        double parsing_median = parsing_results[NO_TESTS / 2];

        std::sort(processing_results.begin(), processing_results.end());
        double processing_median = processing_results[NO_TESTS / 2];
        printf("%d,%f,%f\n", test_size, parsing_median, processing_median);
    }

    return 0;
}
