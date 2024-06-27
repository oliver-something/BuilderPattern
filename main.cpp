#include <format>
#include <functional>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

class Actor {
private:
    int age_;
    std::string first_name_;
    std::string last_name_;

public:
    explicit Actor(int age, std::string_view firstName, std::string_view lastname):age_(age), first_name_(firstName), last_name_(lastname) {}
    [[nodiscard]] std::string ToString() const {
        return std::format("first name: {}, last name : {}, age : {}\n", this->first_name_, this->last_name_, this->age_);
    }
};

class ActorBuilder {
private:
    int age_{};
    std::string first_name_;
    std::string last_name_;

private:
    ActorBuilder() = default;
public:
    static ActorBuilder Builder() {
        return {};
    }

    ActorBuilder WithFirstName(std::string_view firstName) {
        this->first_name_ = firstName;
        return *this;
    }

    ActorBuilder WithLastName(std::string_view lastName) {
        this->last_name_ = lastName;
        return *this;
    }

    ActorBuilder BornIn(int age) {
        this->age_ = age;
        return *this;
    }

    Actor Build() {
        return Actor(age_, first_name_, last_name_);
    }
};

class Movie {
private:
    std::string title_;
    std::string director_;
    int release_year_;
    std::vector<Actor> actors_;
public:
    explicit Movie(std::string_view title, std::string_view director, int releaseYear, std::vector<Actor> actors) : title_(title),
                                                                                                                    director_(director),
                                                                                                                    actors_(std::move(actors)),
                                                                                                                    release_year_(releaseYear) {}
    [[nodiscard]] std::string ToString() const {
        return std::format("Title: {}, Director: {}, Release Date: {}\n", this->title_, this->director_, this->release_year_);
    }

    void Casts() {
        for (auto & actor:actors_) {
            std::cout << actor.ToString();
        }
    }
};

class MovieBuilder {
private:
    std::string title_;
    std::string director_;
    int release_year_{};
    std::vector<Actor> actors_;
private:
    MovieBuilder() {
        actors_ = std::vector<Actor>();
    };
public:
    static MovieBuilder Builder() {
        return {};
    }

    MovieBuilder WithTitle(std::string_view title) {
        this->title_ = title;
        return *this;
    }

    MovieBuilder WithDirector(std::string_view director) {
        this->director_ = director;
        return *this;
    }

    MovieBuilder ReleasedOn(int release_year) {
        this->release_year_ = release_year;
        return *this;
    }

    MovieBuilder WithActor(const Actor & actor) {
        this->actors_.push_back(actor);
        return *this;
    }

    MovieBuilder& WithActor(const std::function<ActorBuilder (ActorBuilder)>& action) {
        auto actorBuilder = ActorBuilder::Builder();
        auto actor = action(actorBuilder).Build();
        actors_.push_back(actor);
        return *this;
    }


    Movie Build() {
        return Movie(title_, director_, release_year_, actors_);
    }
};

int main() {
    auto movie = MovieBuilder::Builder()
                         .WithTitle("Inception")
                         .WithDirector("Christofer Nolan")
                         .ReleasedOn(2010)
                         .WithActor(ActorBuilder::Builder()
                                            .WithFirstName("Leonardo")
                                            .WithLastName("DiCaprio")
                                            .BornIn(1996).Build())
                         .WithActor([](ActorBuilder actorBuilder) {
                             return actorBuilder.WithFirstName("Oliver").WithLastName("Something").BornIn(1998);
                         })
                         .Build();

    std::cout << movie.ToString();
    std::cout << "-------------\n";
    movie.Casts();

}