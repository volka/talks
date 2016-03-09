#pragma once

#include <sqlpp11/sqlpp11.h>

namespace notes
{
namespace db
{
namespace sqlpp_model
{

// notes table
namespace notes_
{
struct id {
    struct _alias_t {
        static constexpr const char _literal[] = "id";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T> struct _member_t {
            T id;
            T &operator()() { return id; }
            const T &operator()() const { return id; }
        };
    };
    using _traits =
        sqlpp::make_traits<sqlpp::integer, sqlpp::tag::must_not_insert,
                           sqlpp::tag::must_not_update>;
};

struct title {
    struct _alias_t {
        static constexpr const char _literal[] = "title";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T> struct _member_t {
            T title;
            T &operator()() { return title; }
            const T &operator()() const { return title; }
        };
    };
    using _traits = sqlpp::make_traits<sqlpp::varchar, sqlpp::tag::can_be_null>;
};

struct content {
    struct _alias_t {
        static constexpr const char _literal[] = "content";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T> struct _member_t {
            T content;
            T &operator()() { return content; }
            const T &operator()() const { return content; }
        };
    };
    using _traits = sqlpp::make_traits<sqlpp::varchar, sqlpp::tag::can_be_null>;
};

struct notebook {
    struct _alias_t {
        static constexpr const char _literal[] = "notebook";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T> struct _member_t {
            T notebook;
            T &operator()() { return notebook; }
            const T &operator()() const { return notebook; }
        };
    };
    using _traits =
        sqlpp::make_traits<sqlpp::integer, sqlpp::tag::can_be_null>;
};

struct last_change {
    struct _alias_t {
        static constexpr const char _literal[] = "last_change";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T> struct _member_t {
            T last_change;
            T &operator()() { return last_change; }
            const T &operator()() const { return last_change; }
        };
    };
    using _traits =
        sqlpp::make_traits<sqlpp::time_point, sqlpp::tag::can_be_null>;
};

struct reminder {
    struct _alias_t {
        static constexpr const char _literal[] = "reminder";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T> struct _member_t {
            T reminder;
            T &operator()() { return reminder; }
            const T &operator()() const { return reminder; }
        };
    };
    using _traits =
        sqlpp::make_traits<sqlpp::time_point, sqlpp::tag::can_be_null>;
};
}

struct Notes : sqlpp::table_t<Notes, notes_::id, notes_::title, notes_::content,
                              notes_::notebook, notes_::last_change,
                              notes_::reminder> {
    struct _alias_t {
        static constexpr const char _literal[] = "notes";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T> struct _member_t {
            T notes;
            T &operator()() { return notes; }
            const T &operator()() const { return notes; }
        };
    };
};

// notebooks table
namespace notebooks_
{
struct id {
    struct _alias_t {
        static constexpr const char _literal[] = "id";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T> struct _member_t {
            T id;
            T &operator()() { return id; }
            const T &operator()() const { return id; }
        };
    };
    using _traits =
        sqlpp::make_traits<sqlpp::integer, sqlpp::tag::must_not_insert,
                           sqlpp::tag::must_not_update>;
};
struct title {
    struct _alias_t {
        static constexpr const char _literal[] = "title";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T> struct _member_t {
            T title;
            T &operator()() { return title; }
            const T &operator()() const { return title; }
        };
    };
    using _traits = sqlpp::make_traits<sqlpp::varchar, sqlpp::tag::can_be_null>;
};
}

struct Notebooks
    : sqlpp::table_t<Notebooks, notebooks_::id, notebooks_::title> {
    struct _alias_t {
        static constexpr const char _literal[] = "notebooks";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T> struct _member_t {
            T notebooks;
            T &operator()() { return notebooks; }
            const T &operator()() const { return notebooks; }
        };
    };
};

// tags table
namespace tags_
{
struct id {
    struct _alias_t {
        static constexpr const char _literal[] = "id";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T> struct _member_t {
            T id;
            T &operator()() { return id; }
            const T &operator()() const { return id; }
        };
    };
    using _traits =
        sqlpp::make_traits<sqlpp::integer, sqlpp::tag::must_not_insert,
                           sqlpp::tag::must_not_update>;
};
struct title {
    struct _alias_t {
        static constexpr const char _literal[] = "title";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T> struct _member_t {
            T title;
            T &operator()() { return title; }
            const T &operator()() const { return title; }
        };
    };
    using _traits = sqlpp::make_traits<sqlpp::varchar, sqlpp::tag::can_be_null>;
};
}

struct Tags : sqlpp::table_t<Tags, tags_::id, tags_::title> {
    struct _alias_t {
        static constexpr const char _literal[] = "tags";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T> struct _member_t {
            T tags;
            T &operator()() { return tags; }
            const T &operator()() const { return tags; }
        };
    };
};

// tags_nm table
namespace tags_nm_
{
struct tag_id {
    struct _alias_t {
        static constexpr const char _literal[] = "tag_id";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T> struct _member_t {
            T tag_id;
            T &operator()() { return tag_id; }
            const T &operator()() const { return tag_id; }
        };
    };
    using _traits =
        sqlpp::make_traits<sqlpp::integer>;
};

struct note_id {
    struct _alias_t {
        static constexpr const char _literal[] = "note_id";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T> struct _member_t {
            T note_id;
            T &operator()() { return note_id; }
            const T &operator()() const { return note_id; }
        };
    };
    using _traits =
        sqlpp::make_traits<sqlpp::integer>;
};
}

struct Tags_NM : sqlpp::table_t<Tags_NM, tags_nm_::tag_id, tags_nm_::note_id> {
    struct _alias_t {
        static constexpr const char _literal[] = "tags_nm";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template <typename T> struct _member_t {
            T tags_nm;
            T &operator()() { return tags_nm; }
            const T &operator()() const { return tags_nm; }
        };
    };
};

} // ns sqlpp_model
} // ns db
} // ns notes
