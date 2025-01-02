import request from "@/utils/request";

export interface UserQuery {
    username?: string
    phoneNumber?: string
    status?: number
    dateRange: Date[] | string
}

export interface pageQuery {
    page: number
    pageSize: number
}

export interface User {
    user_id: number
    username: string
    nickname: string
    dept: {
        dept_name: string
    }
    phone: string
    status: number
    create_time: string
    selected: boolean
}

export interface PaginateResponse<T> {
    list: T[]
    total: number
    page: number
    pageSize: number
    maxPage: number
}

export const getUserList = (query: UserQuery & pageQuery): Promise<PaginateResponse<User>> => {
    console.log(query)
    if (Array.isArray(query.dateRange)) {
        query.dateRange = query.dateRange.join(',')
    }
    return request.get<PaginateResponse<User>, PaginateResponse<User>, void>(
        '/user',
        {
            params: query
        })
}

export const updateStatus = (userId: number, status: number): Promise<void> => {
    return request.put(`/user/${userId}/status/${status}`)
}