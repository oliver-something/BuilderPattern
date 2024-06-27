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

    ActorBuilder& WithFirstName(std::string_view firstName) {
        this->first_name_ = firstName;
        return *this;
    }

    ActorBuilder& WithLastName(std::string_view lastName) {
        this->last_name_ = lastName;
        return *this;
    }

    ActorBuilder& BornIn(int age) {
        this->age_ = age;
        return *this;
    }

    Actor Build() {
        return Actor(age_, first_name_, last_name_);
    }
};

class Director {
private:
    int age_;
    std::string first_name_;
    std::string last_name_;
public:
    explicit Director(int age, std::string_view firstName, std::string_view lastname):age_(age), first_name_(firstName), last_name_(lastname) {}
    [[nodiscard]] std::string ToString() const {
        return std::format("first name: {}, last name : {}, age : {}\n", this->first_name_, this->last_name_, this->age_);
    }
};

class DirectorBuilder {
private:
    int age_{};
    std::string first_name_;
    std::string last_name_;

private:
    DirectorBuilder() = default;
public:
    static DirectorBuilder Builder() {
        return {};
    }

    DirectorBuilder& WithFirstName(std::string_view firstName) {
        this->first_name_ = firstName;
        return *this;
    }

    DirectorBuilder& WithLastName(std::string_view lastName) {
        this->last_name_ = lastName;
        return *this;
    }

    DirectorBuilder& BornIn(int age) {
        this->age_ = age;
        return *this;
    }

    Director Build() {
        return Director(age_, first_name_, last_name_);
    }
};

class Movie {
private:
    std::string title_;
    int release_year_;
    std::vector<Actor> actors_;
    std::vector<Director> directors_;
public:
    explicit Movie(std::string_view title, std::vector<Director> directors, int releaseYear, std::vector<Actor> actors) : title_(title),
                                                                                                                    directors_(std::move(directors)),
                                                                                                                    actors_(std::move(actors)),
                                                                                                                    release_year_(releaseYear) {}
    [[nodiscard]] std::string ToString() const {
        return std::format("Title: {}, Release Date: {}\n", this->title_, this->release_year_);
    }

    void Casts() {
        for (auto & actor:actors_) {
            std::cout << actor.ToString();
        }
    }

    void Directors() {
        for (auto & director: directors_)
            std::cout << director.ToString();
    }
};

class MovieBuilder {
private:
    std::string title_;
    std::vector<Director> directors_;
    int release_year_{};
    std::vector<Actor> actors_;
private:
    MovieBuilder() {
        actors_ = std::vector<Actor>();
        directors_ = std::vector<Director>();
    };
public:
    static MovieBuilder Builder() {
        return {};
    }

    MovieBuilder& WithTitle(std::string_view title) {
        this->title_ = title;
        return *this;
    }

    MovieBuilder& WithDirector(const std::function<DirectorBuilder(DirectorBuilder)> & action) {
        auto directorBuilder = DirectorBuilder::Builder();
        auto director = action(directorBuilder).Build();
        directors_.push_back(director);
        return *this;
    }

    MovieBuilder& ReleasedOn(int release_year) {
        this->release_year_ = release_year;
        return *this;
    }

    MovieBuilder& WithActor(const std::function<ActorBuilder (ActorBuilder)>& action) {
        auto actorBuilder = ActorBuilder::Builder();
        auto actor = action(actorBuilder).Build();
        actors_.push_back(actor);
        return *this;
    }

    Movie Build() {
        return Movie(title_, directors_, release_year_, actors_);
    }
};

int main() {
    auto movie = MovieBuilder::Builder()
                         .WithTitle("Inception")
                         .ReleasedOn(2010)
                         .WithDirector([](DirectorBuilder directorBuilder) {
                             return directorBuilder.WithFirstName("Christofer").WithLastName("Nolan").BornIn(1960);
                         })
                         .WithActor([](ActorBuilder actorBuilder) {
                             return actorBuilder.WithFirstName("Oliver").WithLastName("Something").BornIn(1998);
                         })
                         .Build();

    std::cout << movie.ToString();
    std::cout << "Actors-------------\n";
    movie.Casts();
    std::cout << "Directors_____________\n";
    movie.Directors();
}